#include "index.h"
#include "login.h"
#include "signup.h"
#include "active.h"
#include "room.h"
#include "game.h"
#include "roomInfo.h"
int menuIndex = 1;
char username[255];
char password[255];
char message[255];
int inRoom = 0;

bool checkOnlineUser()
{
    Dllist userList = makeUsersList("users.txt");

    // Loop through the userList and check online status for each user
    Dllist temp;
    dll_traverse(temp, userList)
    {
        User user = (User)jval_v(dll_val(temp));

        // Check if the user is online using the session management code
        bool online = isUserOnline(user->username);

        if (online)
        {
            printf("User %s is online.\n", user->username);
        }
    }
}

void baseMenu()
{
    while (1)
    {
        if (menuIndex != 1)
            break;

        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Activate Account\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            printf("Enter username: ");
            scanf("%s", username);

            printf("Enter password: ");
            scanf("%s", password);
            int status = processSignup(username, password, message);
            printf("%s\n", message);
            break;
        }
        case 2:
        {
            printf("Enter username: ");
            scanf("%s", username);

            printf("Enter password: ");
            scanf("%s", password);
            int status = processLogin(username, password, message);
            printf("%s\n", message);
            if (status)
            {
                menuIndex = 2;
            }
            break;
        }
        case 3:
        {
            char keycode[255];
            printf("Enter username: ");
            scanf("%s", username);

            printf("Enter password: ");
            scanf("%s", password);

            printf("Enter admin private code: ");
            scanf("%s", keycode);
            int status = processActive(username, password, keycode, message);
            printf("%s\n", message);
            break;
        }
        case 4:
        {
            menuIndex = 0;
        }
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void subMenu()
{
    int choice;

    while (1)
    {
        if (menuIndex != 2)
            break;

        printf("\nSubmenu Options:\n");
        printf("1. List Online Players\n");
        printf("2. List Rooms\n");
        printf("3. Create Room\n");
        printf("4. Find Room\n");
        printf("5. Join Room\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        if (choice == 6)
        {
            menuIndex = 0;
        }
        switch (choice)
        {
        case 1:
        {
            checkOnlineUser();
            break;
        }
        case 2:
        {
            char roomName[255];
            int room_size = 0;
            RoomInfo room[100];
            int status = processFindRoom("", &room_size, room);
            if (status)
            {
                char str[10] = "    ";
                printf("%-5s %-15s %-15s %-8s %-10s \n", "ID", "Name", "Owner", "Player", "Status");
                for (int i = 0; i < room_size; i++)
                {
                    str[0] = room[i].currentPlayer + '0';
                    str[1] = '/';
                    str[2] = room[i].maxPlayer + '0';
                    printf("%-5d %-15s %-15s %-8s %-10s \n", room[i].id, room[i].roomName, room[i].owner, str, room[i].status ? "Playing" : "Waiting");
                }
            }
            break;
        }
        case 3:
        {
            char roomName[255];
            int maxPlayer;
            char message[255];
            printf("Enter room name : ");
            scanf("%s", roomName);
            printf("Enter max player (2-4) : ");
            scanf("%d", &maxPlayer);
            int status = processCreateRoom(roomName, maxPlayer, message);
            // handle display user in room
            printf("%s\n", message);
            printf("You are in room %s \n", roomName);
            inRoom = 1;
            menuIndex = 3;
            break;
        }
        case 4:
        {
            char roomName[255];
            int room_size = 0;
            char message[255];
            RoomInfo room[10];
            printf("Enter room name : ");
            scanf("%s", roomName);
            int status = processFindRoom(roomName, &room_size, room);
            if (room_size)
            {
                char str[10] = "    ";
                printf("%-5s %-15s %-15s %-8s %-10s \n", "ID", "Name", "Owner", "Player", "Status");
                for (int i = 0; i < room_size; i++)
                {
                    str[0] = room[i].currentPlayer + '0';
                    str[1] = '/';
                    str[2] = room[i].maxPlayer + '0';
                    printf("%-5d %-15s %-15s %-8s %-10s \n", room[i].id, room[i].roomName, room[i].owner, str, room[i].status ? "Playing" : "Waiting");
                }
            }
            else
            {
                printf("Not found any room");
            }
            break;
        }
        case 5:
        {
            char roomName[255];
            char message[255];
            printf("Enter room name : ");
            scanf("%s", roomName);
            int status = processJoinRoom(roomName, message);
            printf("%s\n", message);
            if (status)
            {
                inRoom = 1;
                menuIndex = 3;
                printf("You are in room %s \n", roomName);
            }
            break;
        }
        default:
            break;
        }
    }
}

void inGameMenu()
{
    int choice;
    char chat[255] = "";
    char user[255] = "";
    while (1)
    {
        if (chat != "" && user != "")
        {
            printf("%s said that : %s \n", user, chat);
        }

        printf("\nIn-Game Options:\n");
        printf("1. Start Game\n");
        printf("2. Chat\n");
        printf("3. Chat\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            // printf("Starting the game...\n");
            // int status = playClick(currentRoom,message);
            // break;
        case 2:
        {
            printf("Enter chat...\n");
            scanf("%s", chat);
            processChat(chat, user);
            break;
        }
        case 3:
        {
            printf("Leaving room... \n");
            int status = processLeaveRoom(message);
            printf("%s\n", message);
        }
        case 4:
        {
            menuIndex = 0;
            printf("Exiting program.\n");
            return;
        }
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage: ./client <server ip> <server port>\n");
        exit(0);
    }
    int clientfd = initConnectionClient(argv[1], atoi(argv[2]));
    while (1)
    {
        switch (menuIndex)
        {
        case 1:
            baseMenu();
            break;
        case 2:
            subMenu();
            break;
        case 3:
            inGameMenu();
            break;

        default:
            break;
        }
        if (menuIndex == 0)
        {
            printf("Exiting program.\n");
            break;
        }
    }

    return 0;
}