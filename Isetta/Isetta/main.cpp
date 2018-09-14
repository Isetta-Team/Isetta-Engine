/*
 * Copyright (c) 2018 Isetta
 */
#include <chrono>
#include <string>
#include "Audio/AudioSource.h"
#include "Core/Config/Config.h"
#include "Core/Debug/Logger.h"
#include "Core/Math/Random.h"
#include "Core/Math/Vector3.h"
#include "Core/Memory/Memory.h"
#include "Core/ModuleManager.h"
#include "Core/Time.h"
#include "Graphics/LightNode.h"
#include "Graphics/ModelNode.h"
#include "Input/Input.h"
#include "Networking/NetworkingModule.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <signal.h>
#include "yojimbo/shared.h"

using namespace Isetta;

float Time::time = 0;
std::chrono::time_point<std::chrono::high_resolution_clock> Time::startTime;
float Time::deltaTime = 0;
int Time::frameCount = 0;

/*! \mainpage Isetta Engine
Game engine development is a very wide field in the industry, but also a very
inaccessible one. Budding engineers are advised to just jump into development to
learn, and it's for this reason that the Isetta Engine project came to be.

Our team is building a game engine specialized for the twin-stick shooter genre.
Every step of the way we will be documenting our process through a dedicated
blog. Readers can hear from us personally and get an in-depth look at our
journey to create the engine. In addition, we will supplement our content
through regular interviews with industry professionals who have built their own
engines before.

Between our own hands-on process and sage advice from veteran engineers, we hope
to give newcomers a clearer representation of the engine-building process.
*/
int main() {
  Config config;
  config.Read("config.cfg");
  LOG_INFO(Debug::Channel::General,
           config.vector3Var.GetVal().ToString().c_str());

  ModuleManager moduleManager;
  moduleManager.StartUp();

  // Random number test
  // auto rnd = Math::Random::GetRandomGenerator(1.f, 10.f);
  // float number = rnd.GetValue();
  // Logger::Log(Debug::Channel::General,
  // "Random number: " + std::to_string(number));

  using clock = std::chrono::high_resolution_clock;
  typedef std::chrono::duration<float> second;

  auto audio = new AudioSource();
  audio->SetAudioClip("singing.wav");
  audio->Play(false, 1.0f);

  // Benchmarking
  const int testIterations = 10;
  for (int a = 0; a < testIterations; a++) {
    const auto benchmarkStart = clock::now();
    // benchmark code here...
    const auto benchmarkEnd = clock::now();
    LOG_INFO(
        Debug::Channel::Memory,
        {"Bench mark result: ",
         std::to_string(second(benchmarkEnd - benchmarkStart).count()), "s"});
  }

  // Game loop
  Time::startTime = clock::now();
  auto lastFrameStartTime = clock::now();

  // Connect a client and a client-server together
   if (!InitializeYojimbo() /* from yojimbo.h */) {
    printf("error: failed to initialize Yojimbo!\n");
    return 1;
   }

   const int MaxPacketSize = 16 * 1024;
   const int MaxSnapshotSize = 8 * 1024;
   const int MaxBlockSize = 64 * 1024;

   const int YOJ_UNORDERED = 0;
   const int YOJ_ORDERED = 1;

   const int yojimboServerPort = 40000;
   const int yojimboMaxClients = 64;

   double yTime = 0.0;  // TODO: Dunno how to get yojimbo working off of our time yet
   double yTimeInc = .1;

   yojimbo::ClientServerConfig yojimboConfig;
   yojimboConfig.maxPacketSize = MaxPacketSize;
   yojimboConfig.clientMemory = 10 * 1024 * 1024;
   yojimboConfig.serverGlobalMemory = 10 * 1024 * 1024;
   yojimboConfig.serverPerClientMemory = 10 * 1024 * 1024;
   yojimboConfig.numChannels = 2;
   yojimboConfig.channel[YOJ_UNORDERED].type =
       CHANNEL_TYPE_UNRELIABLE_UNORDERED;
   yojimboConfig.channel[YOJ_UNORDERED].maxBlockSize = MaxSnapshotSize;
   yojimboConfig.channel[YOJ_ORDERED].type =
       CHANNEL_TYPE_RELIABLE_ORDERED;
   yojimboConfig.channel[YOJ_ORDERED].maxBlockSize = MaxBlockSize;
   yojimboConfig.channel[YOJ_ORDERED].blockFragmentSize = 1024;

   uint8_t privateKey[KeyBytes];
   memset(privateKey, 0, KeyBytes);

   double time = 0.0;

   Address serverAddress("127.0.0.1", ServerPort);

   Server server(yojimbo::GetDefaultAllocator(), privateKey, serverAddress,
                 yojimboConfig,
                 adapter, time);

   server.Start(1);

   if (!server.IsRunning()) {
     LOG(Debug::Channel::General, "Server ain't running!");
     return 1;
   }

   uint64_t clientId = 0;
   random_bytes((uint8_t *)&clientId, 8);

   Client client(yojimbo::GetDefaultAllocator(), Address("0.0.0.0"), yojimboConfig, adapter,
                 time);

   client.InsecureConnect(privateKey, clientId, serverAddress);

   uint64_t yojMessagesToServer = 0;
   uint64_t yojMessagesToClient = 0;
   uint64_t yojMessagesFromServer = 0;
   uint64_t yojMessagesFromClient = 0;

   bool yojClientConnected = false;
   bool yojServerConnected = false;

   bool clientIsSending = true;
   bool networkKeyPressed = false;

  // play first audio clip
  auto audioSource = new AudioSource();
  audioSource->SetAudioClip("wave.mp3");

  audioSource->Play(true, 1.0f);
  ModelNode car{"test/Low-Poly-Racing-Car.scene.xml", Math::Vector3{0, -20, 0},
                Math::Vector3::zero, Math::Vector3::one};

  LightNode light{"materials/light.material.xml", Math::Vector3{0, 200, 600},
                  Math::Vector3::zero, Math::Vector3::one};

  bool running{true};

  while (running) {
    // Networking
    server.SendPackets();
    client.SendPackets();

    server.ReceivePackets();
    client.ReceivePackets();

    Time::deltaTime = second(clock::now() - lastFrameStartTime).count();
    Time::time = second(clock::now() - Time::startTime).count();
    lastFrameStartTime = clock::now();

    moduleManager.Update();
    Time::frameCount++;

    // Networking part 2: Electric boogaloo
    yTime += yTimeInc;
    if (client.IsConnected()) {
      yojClientConnected = true;
      {
        if (clientIsSending) {
          if ((Input::IsKeyPressed(KeyCode::I) ||
              Input::IsKeyPressed(KeyCode::O))) {
            if (!networkKeyPressed) {
              TestMessage *message = (TestMessage *)client.CreateMessage(
                  TEST_MESSAGE); /* from shared.h */
              if (message) {
                message->sequence =
                    uint16_t(Input::IsKeyPressed(KeyCode::I) ? 'I' : 'O');
                client.SendMessage(YOJ_ORDERED, message);
                yojMessagesToServer++;
                clientIsSending = false;
                networkKeyPressed = true;
              }
            }
          } else {
            networkKeyPressed = false;
          }
        }
      }

      const int clientIndex = client.GetClientIndex();

      if (server.IsClientConnected(clientIndex)) {
        yojServerConnected = true;
        {
          if (!clientIsSending) {
            if ((Input::IsKeyPressed(KeyCode::I) ||
                 Input::IsKeyPressed(KeyCode::O))) {
              if (!networkKeyPressed) {
                TestMessage *message = (TestMessage *)server.CreateMessage(clientIndex,
                    TEST_MESSAGE); /* from shared.h */
                if (message) {
                  message->sequence =
                      uint16_t(Input::IsKeyPressed(KeyCode::I) ? 'I' : 'O');
                  server.SendMessage(clientIndex, YOJ_ORDERED, message);
                  yojMessagesToClient++;
                  clientIsSending = true;
                  networkKeyPressed = true;
                }
              }
            } else {
              networkKeyPressed = false;
            }
          }
        }

        while (true) {
          yojimbo::Message *message = server.ReceiveMessage(clientIndex, YOJ_ORDERED);

          if (!message) {
            break;
          }

          yojimbo_assert(message->GetId() == (uint16_t)yojMessagesFromClient);

          switch (message->GetType()) {
            case TEST_MESSAGE: {
              TestMessage *testMessage =
                  (TestMessage *)message; /* from shared.h */
              LOG(Debug::Channel::Networking, Debug::Verbosity::Info,
                  "server received message %c\n", testMessage->sequence);
              server.ReleaseMessage(clientIndex, message);
              yojMessagesFromClient++;
            } break;

            case TEST_BLOCK_MESSAGE: {
              TestBlockMessage *blockMessage =
                  (TestBlockMessage *)message; /* from shared.h */
              const int blockSize = blockMessage->GetBlockSize();
              const int expectedBlockSize =
                  1 + (int(yojMessagesFromClient) * 33) % MaxBlockSize;
              if (blockSize != expectedBlockSize) {
                LOG(Debug::Channel::Networking, Debug::Verbosity::Error,
                    "error: block size mismatch. expected %d, got %d\n",
                    expectedBlockSize, blockSize);
                yojServerConnected = false;
                break;
              }
              const uint8_t *blockData = blockMessage->GetBlockData();
              yojimbo_assert(blockData);
              for (int i = 0; i < blockSize; i++) {
                if (blockData[i] != (uint8_t)yojMessagesFromClient + i) {
                  LOG(Debug::Channel::Networking, Debug::Verbosity::Error,
                      "error: block data mismatch. expected %d, but "
                      "blockData[%d] = %c\n",
                      uint8_t(yojMessagesFromClient + i), i, blockData[i]);
                  yojServerConnected = false;
                  break;
                }
              }
              LOG(Debug::Channel::Networking, Debug::Verbosity::Info,
                  "server received message %c\n",
                  (uint16_t)yojMessagesFromClient);
              server.ReleaseMessage(clientIndex, message);
              yojMessagesFromClient++;
            } break;
          }
        }

        while (true) {
          yojimbo::Message *message = client.ReceiveMessage(YOJ_ORDERED);

          if (!message) {
            break;
          }

          yojimbo_assert(message->GetId() == (uint16_t)yojMessagesFromServer);

          switch (message->GetType()) {
            case TEST_MESSAGE: {
              TestMessage *testMessage =
                  (TestMessage *)message; /* from shared.h */
              LOG(Debug::Channel::Networking, Debug::Verbosity::Info,
                  "client received message %c\n", testMessage->sequence);
              client.ReleaseMessage(message);
              yojMessagesFromServer++;
            } break;

            case TEST_BLOCK_MESSAGE: {
              TestBlockMessage *blockMessage =
                  (TestBlockMessage *)message; /* from shared.h */
              const int blockSize = blockMessage->GetBlockSize();
              const int expectedBlockSize =
                  1 + (int(yojMessagesFromServer) * 33) % MaxBlockSize;
              if (blockSize != expectedBlockSize) {
                LOG(Debug::Channel::Networking, Debug::Verbosity::Error,
                    "error: block size mismatch. expected %d, got %d\n",
                    expectedBlockSize, blockSize);
                yojServerConnected = false;
                break;
              }
              const uint8_t *blockData = blockMessage->GetBlockData();
              yojimbo_assert(blockData);
              for (int i = 0; i < blockSize; i++) {
                if (blockData[i] != (uint8_t)yojMessagesFromServer + i) {
                  LOG(Debug::Channel::Networking, Debug::Verbosity::Error,
                      "error: block data mismatch. expected %d, but "
                      "blockData[%d] = %c\n",
                      uint8_t(yojMessagesFromServer + i), i, blockData[i]);
                  yojServerConnected = false;
                  break;
                }
              }
              LOG(Debug::Channel::Networking, Debug::Verbosity::Info,
                  "client received message %c\n",
                  (uint16_t)yojMessagesFromServer);
              client.ReleaseMessage(message);
              yojMessagesFromServer++;
            } break;
          }
        }
      }
    }

    if (client.IsDisconnected() && yojClientConnected) {
      yojClientConnected = false;
      LOG(Debug::Channel::Networking, Debug::Verbosity::Info,
                    "client %.16 " PRIx16
                    " has been disconnected from server!\n");
    }

    // switch to playing the second audio clip
    if (Time::frameCount == 1000000) {
      audioSource->Stop();
      audioSource->SetAudioClip("singing.wav");
      audioSource->Play(false, 1.0f);
    }

    client.AdvanceTime(yTime);
    server.AdvanceTime(yTime);

    if (Input::IsKeyPressed(KeyCode::ESCAPE)) {
      running = false;
    }
  }

  // Networking
   client.Disconnect();
   server.Stop();

  moduleManager.ShutDown();
  return 0;
}
