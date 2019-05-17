#pragma once
#include "ChiliWin.h"
// need to specify DXSDK over WinSDK (do not want to target win8)
// added $(DXSDK_DIR)\include
#include <xaudio2.h>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <vector>
#include <mutex>
#include <atomic>
#include "ComManager.h"
#include <wrl\client.h>

class SoundSystem
{
public:
	class Error : public std::runtime_error
	{
	public:
		Error(std::string s)
			:
			runtime_error(std::string("SoundSystem::Error: ") + s)
		{}
	};
	class FileError : public Error
	{
	public:
		FileError(std::string s)
			:
			Error(std::string("SoundSystem::FileError: ") + s)
		{}
	};

public:
	class Channel
	{
	private:
		class VoiceCallback : public IXAudio2VoiceCallback
		{
		public:
			void STDMETHODCALLTYPE OnStreamEnd() override
			{}
			void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override
			{}
			void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) override
			{}
			void STDMETHODCALLTYPE OnBufferEnd(void* pBufferContext) override;
			void STDMETHODCALLTYPE OnBufferStart(void* pBufferContext) override
			{}
			void STDMETHODCALLTYPE OnLoopEnd(void* pBufferContext) override
			{}
			void STDMETHODCALLTYPE OnVoiceError(void* pBufferContext, HRESULT Error) override
			{}
		};

	public:
		Channel(SoundSystem& sys)
		{
			static VoiceCallback vcb;
			ZeroMemory(&xaBuffer, sizeof(xaBuffer));
			xaBuffer.pContext = this;
			sys.pEngine->CreateSourceVoice(&pSource, &sys.format, 0u, 2.0f, &vcb);
		}
		Channel(const Channel&) = delete;
		~Channel()
		{
			assert(!pSound);
			if (pSource)
			{
				pSource->DestroyVoice();
				pSource = nullptr;
			}
		}
		void PlaySoundBuffer(class Sound& s, float freqMod, float vol);
		void Stop()
		{
			assert(pSource && pSound);
			pSource->Stop();
			pSource->FlushSourceBuffers();
		}

	private:
		XAUDIO2_BUFFER xaBuffer;
		IXAudio2SourceVoice* pSource = nullptr;
		// does this need to be synchronized?
		// (no--no overlap of callback thread and main thread here)
		class Sound* pSound = nullptr;
	};

public:
	SoundSystem(const SoundSystem&) = delete;
	static SoundSystem& Get();
	static WAVEFORMATEX& GetFormat()
	{
		return Get().format;
	}
	void PlaySoundBuffer(class Sound& s, float freqMod, float vol)
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (idleChannelPtrs.size() > 0)
		{
			activeChannelPtrs.push_back(std::move(idleChannelPtrs.back()));
			idleChannelPtrs.pop_back();
			activeChannelPtrs.back()->PlaySoundBuffer(s, freqMod, vol);
		}
	}

private:
	SoundSystem();
	void DeactivateChannel(Channel& channel)
	{
		std::lock_guard<std::mutex> lock(mutex);
		auto i = std::find_if(activeChannelPtrs.begin(), activeChannelPtrs.end(),
			[&channel](const std::unique_ptr<Channel> & pChan) -> bool
			{
				return &channel == pChan.get();
			});
		idleChannelPtrs.push_back(std::move(*i));
		activeChannelPtrs.erase(i);
	}

private:
	ComManager comMan;
	Microsoft::WRL::ComPtr<IXAudio2> pEngine;
	IXAudio2MasteringVoice* pMaster = nullptr;
	WAVEFORMATEX format;
	const int nChannels = 64;
	std::mutex mutex;
	std::vector<std::unique_ptr<Channel>> idleChannelPtrs;
	std::vector<std::unique_ptr<Channel>> activeChannelPtrs;
};

class Sound
{
	friend SoundSystem::Channel;

public:
	Sound(const std::wstring& fileName)
	{
		unsigned int fileSize = 0;
		std::unique_ptr<BYTE[]> pFileIn;
		try
		{
			{
				std::ifstream file;
				file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
				try
				{
					file.open(fileName, std::ios::binary);
				}
				catch (std::exception e)
				{
					throw SoundSystem::FileError(
						std::string("error opening file: ") + e.what());
				}

				{
					int fourcc;
					file.read(reinterpret_cast<char*>(&fourcc), 4);
					if (fourcc != 'FFIR')
					{
						throw SoundSystem::FileError("bad fourCC");
					}
				}

				file.read(reinterpret_cast<char*>(&fileSize), 4);
				fileSize += 8; // entry doesn't include the fourcc or itself
				if (fileSize <= 44)
				{
					throw SoundSystem::FileError("file too small");
				}

				file.seekg(0, std::ios::beg);
				pFileIn = std::make_unique<BYTE[]>(fileSize);
				file.read(reinterpret_cast<char*>(pFileIn.get()), fileSize);
			}

			if (*reinterpret_cast<const int*>(&pFileIn[8]) != 'EVAW')
			{
				throw SoundSystem::FileError("format not WAVE");
			}

			//look for 'fmt ' chunk id
			WAVEFORMATEX format;
			bool bFilledFormat = false;
			for (unsigned int i = 12; i < fileSize; )
			{
				if (*reinterpret_cast<const int*>(&pFileIn[i]) == ' tmf')
				{
					memcpy(&format, &pFileIn[i + 8u], sizeof(format));
					bFilledFormat = true;
					break;
				}
				// chunk size + size entry size + chunk id entry size + word padding
				i += (*reinterpret_cast<const int*>(&pFileIn[i + 4u]) + 9) & 0xFFFFFFFE;
			}
			if (!bFilledFormat)
			{
				throw SoundSystem::FileError("fmt chunk not found");
			}

			// compare format with sound system format
			{
				const WAVEFORMATEX sysFormat = SoundSystem::GetFormat();

				if (format.nChannels != sysFormat.nChannels)
				{
					throw SoundSystem::FileError("bad wave format (nChannels)");
				}
				else if (format.wBitsPerSample != sysFormat.wBitsPerSample)
				{
					throw SoundSystem::FileError("bad wave format (wBitsPerSample)");
				}
				else if (format.nSamplesPerSec != sysFormat.nSamplesPerSec)
				{
					throw SoundSystem::FileError("bad wave format (nSamplesPerSec)");
				}
				else if (format.wFormatTag != sysFormat.wFormatTag)
				{
					throw SoundSystem::FileError("bad wave format (wFormatTag)");
				}
				else if (format.nBlockAlign != sysFormat.nBlockAlign)
				{
					throw SoundSystem::FileError("bad wave format (nBlockAlign)");
				}
				else if (format.nAvgBytesPerSec != sysFormat.nAvgBytesPerSec)
				{
					throw SoundSystem::FileError("bad wave format (nAvgBytesPerSec)");
				}
			}

			//look for 'data' chunk id
			bool bFilledData = false;
			for (unsigned int i = 12; i < fileSize; )
			{
				const int chunkSize = *reinterpret_cast<const int*>(&pFileIn[i + 4u]);
				if (*reinterpret_cast<const int*>(&pFileIn[i]) == 'atad')
				{
					pData = std::make_unique<BYTE[]>(chunkSize);
					nBytes = chunkSize;
					memcpy(pData.get(), &pFileIn[i + 8u], nBytes);

					bFilledData = true;
					break;
				}
				// chunk size + size entry size + chunk id entry size + word padding
				i += (chunkSize + 9) & 0xFFFFFFFE;
			}
			if (!bFilledData)
			{
				throw SoundSystem::FileError("data chunk not found");
			}
		}
		catch (SoundSystem::Error e)
		{
			throw e;
		}
		catch (std::exception e)
		{
			throw SoundSystem::FileError(e.what());
		}
	}
	Sound(const Sound&) = delete;
	Sound(Sound&& donor) noexcept
		:
		nBytes(donor.nBytes),
		pData(std::move(donor.pData)),
		activeChannelPtrs(std::move(donor.activeChannelPtrs))
	{}
	void Play(float freqMod, float vol)
	{
		SoundSystem::Get().PlaySoundBuffer(*this, freqMod, vol);
	}
	~Sound()
	{
		{
			std::lock_guard<std::mutex> lock(mutex);
			for (auto pChannel : activeChannelPtrs)
			{
				pChannel->Stop();
			}
		}

		bool allChannelsDeactivated = false;
		do
		{
			std::lock_guard<std::mutex> lock(mutex);
			allChannelsDeactivated = activeChannelPtrs.size() == 0;
		} while (!allChannelsDeactivated);
	}

private:
	void RemoveChannel(SoundSystem::Channel& channel)
	{
		std::lock_guard<std::mutex> lock(mutex);
		activeChannelPtrs.erase(std::find(
			activeChannelPtrs.begin(), activeChannelPtrs.end(), &channel));
	}
	void AddChannel(SoundSystem::Channel& channel)
	{
		std::lock_guard<std::mutex> lock(mutex);
		activeChannelPtrs.push_back(&channel);
	}

private:
	UINT32 nBytes = 0;
	std::unique_ptr<BYTE[]> pData;
	std::mutex mutex;
	std::vector<SoundSystem::Channel*> activeChannelPtrs;
};