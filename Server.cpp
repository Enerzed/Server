﻿// Server.cpp: определяет точку входа для приложения.

#pragma execution_character_set("utf-8")

#include "Server.h"

void Server::Run()
{
    ServerNetwork serverNetwork();
    // Окно
    sf::RenderWindow window(sf::VideoMode(1280, 720), L"Мессенджер Сервер", sf::Style::Close);
    window.setFramerateLimit(120);

    Interface interface;
    interface.Init(window);
    // Время
    sf::Clock clock;
    // Основной цикл
    while (window.isOpen())
    {
        sf::Time time = clock.getElapsedTime();
        clock.restart();
        // Обработка событий
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                
                window.close();
            }
        }
        // Сеть
        for (size_t iterator = 0; iterator < networks.size(); iterator++)
        {
            networks[iterator]->Run();
        }
        // Передаем системные сообщения
        for (size_t iterator = 0; iterator < networks.size(); iterator++)
        {
            for (size_t iterator2 = 0; iterator2 < networks[iterator]->GetSystemMessages().size(); iterator2++)
            {
                std::string message = networks[iterator]->GetSystemMessages()[iterator2];
                interface.ModifyTextBoxSystemMessage(message, iterator + basePort);
            }
            networks[iterator]->ClearSystemMessages();
        }
        // Передаем пакеты
        for (size_t iterator = 0; iterator < networks.size(); iterator++)
        {
            std::vector<sf::Packet> packets = networks[iterator]->GetPackets();
            for (size_t iterator2 = 0; iterator2 < packets.size(); iterator2++)
            {
                size_t type;
                std::string name;
                std::string message;
                std::string address;
                unsigned short port;

                packets[iterator2] >> type >> name >> message >> address >> port;
                interface.ModifyTextBox(message, name, iterator + basePort);
            }
            networks[iterator]->ClearPackets();
        }
        // Обновляем интерфейс
        interface.Update(window, time);

        if (interface.GetIsCreateServerNetwork() == true)
        {
            interface.AddTextBox(port);
            networks.push_back(new ServerNetwork(port++));
            interface.SetIsCreateServerNetwork(false);
        }
        // Очищаем окно
        window.clear();
        // Render интерфейса должен быть перед показом его на экране для того, чтобы он был на первом плане
        ImGui::SFML::Render(window);
        // Показываем окно
        window.display();
    }
    // Отключаем ImGui-SFML после закрытия окна
    ImGui::SFML::Shutdown();
}


int main()
{
    system("chcp 65001");

    Server server;
    server.Run();

    return 0;
}