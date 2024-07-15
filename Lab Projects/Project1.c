//Project By: Jack Wilcox

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Room Room;
typedef struct Creature Creature;

//Function Prototypes
void createRooms();
void assignNeighbor();
void getRoomState();
void createCreatures();
void assignRoom();
void getCreatureType();
void findPC();
void killCreature();
void creatureAction();
void animalAction();
void humanAction();
Room* moveRooms();
void movePC();
void moveCreature();
void moveCreatureRandomly();
void showCommands();
void look();
void cleanRoom();
void dirtyRoom();

struct Room
{
    int roomID;          //ID of room
    int roomState;       //State of room (0, 1, 2)
    char roomDesc[20];   //Description of room based on roomState (clean, half-dirty, dirty)
    Room* roomNorth;     //Pointer to the north room
    int northRoomID;     //ID of north room
    Room* roomSouth;     //Pointer to the south room
    int southRoomID;     //ID of south room
    Room* roomEast;      //Pointer to the east room
    int eastRoomID;      //ID of east room
    Room* roomWest;      //Pointer to the west room
    int westRoomID;      //ID of west room
    int creatureCounter; //Number of creatures in room
};

struct Creature
{
    int creatureID;    //ID of creature
    int type;          //Type of creature (0, 1, 2)
    char typeDesc[20]; //Description of creature based on type (PC, animal, human)
    Room* currentRoom; //Pointer to the current room of the creature
    int currentRoomID; //ID of the current room of the creature
    bool isAngry;      //Flag for if the creature is angry at an action
    bool controlFlag;  //Flag for if the creature is being controlled by the PC
};

Room* rooms         = NULL; //Pointer to array of rooms
Creature* creatures = NULL; //Pointer to array of creatures
Creature* pc        = NULL; //Pointer to PC

int respect = 40; //PC's initial respect value

//Creates the array of rooms based on user input
void createRooms(int numRms)
{
    for(int i = 0; i < numRms; i++)
    {
        rooms[i].roomID = i;
        rooms[i].creatureCounter = 0;

        scanf("%d %d %d %d %d", &rooms[i].roomState, &rooms[i].northRoomID, &rooms[i].southRoomID, &rooms[i].eastRoomID, &rooms[i].westRoomID);

        getRoomState(&rooms[i]);
    }

    for(int i = 0; i < numRms; i++)
    {
        if(rooms[i].northRoomID != -1)
        {
            assignNeighbor(numRms, rooms[i].northRoomID, &rooms[i].roomNorth);
        }

        else
        {
            rooms[i].roomNorth = NULL;
        }

        if(rooms[i].southRoomID != -1)
        {
            assignNeighbor(numRms, rooms[i].southRoomID, &rooms[i].roomSouth);
        }

        else
        {
            rooms[i].roomSouth = NULL;
        }

        if(rooms[i].eastRoomID != -1)
        {
            assignNeighbor(numRms, rooms[i].eastRoomID, &rooms[i].roomEast);
        }

        else
        {
            rooms[i].roomEast = NULL;
        }

        if(rooms[i].westRoomID != -1)
        {
            assignNeighbor(numRms, rooms[i].westRoomID, &rooms[i].roomWest);
        }

        else
        {
            rooms[i].roomWest = NULL;
        }
    }
}

//Assigns each room its neighbors based on the inputs from the user
void assignNeighbor(int nRms, int neighborID, Room** neighbor)
{
    for(int i = 0; i < nRms; i++)
    {
        if(rooms[i].roomID == neighborID)
        {
            (*neighbor) = &rooms[i];
            (*neighbor)->roomID = rooms[i].roomID;
        }
    }
}

//Assigns each room its descriptive state based off its roomState variable
void getRoomState(Room* r)
{
    if(r->roomState == 0)
    {
        strcpy(r->roomDesc, "clean");
    }

    else if(r->roomState == 1)
    {
        strcpy(r->roomDesc, "half-dirty");
    }

    else if(r->roomState == 2)
    {
        strcpy(r->roomDesc, "dirty");
    }
}

//Creates the array of creatures based on user input
void createCreatures(int numCrtrs, int numRms)
{
    for(int i = 0; i < numCrtrs; i++)
    {
        creatures[i].creatureID = i;

        scanf("%d %d", &creatures[i].type, &creatures[i].currentRoomID);

        getCreatureType(&creatures[i]);

        assignRoom(numRms, &creatures[i]);

        creatures[i].isAngry = false;
        creatures[i].controlFlag = false;
    }
}

//Assigns each creature a room based on their currentRoomID
void assignRoom(int nRms, Creature* curCtr)
{
    for(int i = 0; i < nRms; i++)
    {
        if(rooms[i].roomID == curCtr->currentRoomID)
        {
            rooms[i].creatureCounter++;

            curCtr->currentRoom = &rooms[i];
            curCtr->currentRoomID = rooms[i].roomID;
        }
    }
}

//Assigns each creature its descriptive type based off its type variable
void getCreatureType(Creature* c)
{
    if(c->type == 0)
    {
        strcpy(c->typeDesc, "PC");
    }

    else if(c->type == 1)
    {
        strcpy(c->typeDesc, "animal");
    }

    else if(c->type == 2)
    {
        strcpy(c->typeDesc, "human");
    }
}

//Finds the PC in the creature array and stores it to a global pointer
void findPC(int numCrtrs)
{
    for(int i = 0; i < numCrtrs; i++)
    {
        if(creatures[i].type == 0)
        {
            pc = &creatures[i];
        }
    }
}

//Removes a creature from the world and gets the reactions from the other creatures in that room
void killCreature(Room* curRm, Creature* curCrtr, int nCrtrs)
{
    printf("\n%s %d has left through the ceiling\n", curCrtr->typeDesc, curCrtr->creatureID);

    for(int i = 0; i < nCrtrs; i++)
    {
        if(creatures[i].creatureID != curCrtr->creatureID && creatures[i].currentRoom == curRm)
        {
            creatures[i].isAngry = true;
            creatureAction(&creatures[i]);
        }
    }

    curCrtr->creatureID = -1;
    curCrtr->currentRoomID = -1;
    curCrtr->currentRoom = NULL;
}

//Determines which action the creature will perform based off its type
void creatureAction(Creature* curCrtr)
{
    if(curCrtr->type == 1)
    {
       animalAction(curCrtr);
    }

    else if(curCrtr->type == 2)
    {
        humanAction(curCrtr);
    }
}

//Determines what an animal creature will do based off its anger state and whether its being controlled
void animalAction(Creature* curCrtr)
{
    if(!curCrtr->isAngry)
    {
        if(curCrtr->controlFlag)
        {
            respect = respect + 3;
            printf("\n%d licks your face a lot. Respect is now %d\n", curCrtr->creatureID, respect);
        }

        else
        {
            respect++;
            printf("\n%d licks your face. Respect is now %d\n", curCrtr->creatureID, respect);
        }
    }

    else
    {
        if(curCrtr->controlFlag)
        {
            respect = respect - 3;
            printf("\n%d growls a lot. Respect is now %d\n", curCrtr->creatureID, respect);
        }

        else
        {
            respect--;
            printf("\n%d growls. Respect is now %d\n", curCrtr->creatureID, respect);
        }

        curCrtr->isAngry = false;
    }
}

//Determines what an human creature will do based off its anger state and whether its being controlled
void humanAction(Creature* curCrtr)
{
    if(!curCrtr->isAngry)
    {
        if(curCrtr->controlFlag)
        {
            respect = respect + 3;
            printf("\n%d smiles a lot. Respect is now %d\n", curCrtr->creatureID, respect);
        }

        else
        {
            respect++;
            printf("\n%d smiles. Respect is now %d\n", curCrtr->creatureID, respect);
        }
    }

    else
    {
        if(curCrtr->controlFlag)
        {
            respect = respect - 3;
            printf("\n%d grumbles a lot. Respect is now %d\n", curCrtr->creatureID, respect);
        }

        else
        {
            respect--;
            printf("\n%d grumbles. Respect is now %d\n", curCrtr->creatureID, respect);
        }

        curCrtr->isAngry = false;
    }
}

//Moves the PC or a creature to another viable room and updates the room's creature counter
Room* moveRooms(int nRms, Room* desiredRoom, Room* curRm)
{
    for(int i = 0; i < nRms; i++)
    {
        if(desiredRoom != NULL && rooms[i].roomID == desiredRoom->roomID)
        {
            curRm->creatureCounter--;
            curRm = &rooms[i];
            curRm->creatureCounter++;

            break;
        }
    }

    return curRm;
}

//Determines what room the PC will move to
void movePC(int numRms, char* act)
{
    if(strcmp(act, "north") == 0)
    {
        if(pc->currentRoom->roomNorth != NULL && pc->currentRoom->roomNorth->creatureCounter < 10)
        {
            pc->currentRoom = moveRooms(numRms, pc->currentRoom->roomNorth, pc->currentRoom);
            pc->currentRoomID = pc->currentRoom->roomID;

            printf("\nYou leave towards the north\n");
        }

        else
        {
            printf("\nThe north room doesn't exist or is full\n");
        }
    }

    else if(strcmp(act, "south") == 0)
    {
        if(pc->currentRoom->roomSouth != NULL && pc->currentRoom->roomSouth->creatureCounter < 10)
        {
            pc->currentRoom = moveRooms(numRms, pc->currentRoom->roomSouth, pc->currentRoom);
            pc->currentRoomID = pc->currentRoom->roomID;

            printf("\nYou leave towards the south\n");
        }

        else
        {
            printf("\nThe south room doesn't exist or is full\n");
        }
    }

    else if(strcmp(act, "east") == 0)
    {
        if(pc->currentRoom->roomEast != NULL && pc->currentRoom->roomEast->creatureCounter < 10)
        {
            pc->currentRoom = moveRooms(numRms, pc->currentRoom->roomEast, pc->currentRoom);
            pc->currentRoomID = pc->currentRoom->roomID;

            printf("\nYou leave towards the east\n");
        }

        else
        {
            printf("\nThe east room doesn't exist or is full\n");
        }
    }

    else if(strcmp(act, "west") == 0)
    {
        if(pc->currentRoom->roomWest != NULL && pc->currentRoom->roomWest->creatureCounter < 10)
        {
            pc->currentRoom = moveRooms(numRms, pc->currentRoom->roomWest, pc->currentRoom);
            pc->currentRoomID = pc->currentRoom->roomID;

            printf("\nYou leave towards the west\n");
        }

        else
        {
            printf("\nThe west room doesn't exist or is full\n");
        }
    }
}

//Determines what room a creature controlled by the PC will move to
void moveCreature(Creature* curCrtr, int numRms, int numCrtrs, char* act)
{
    if(strcmp(act, "north") == 0)
    {
        if(curCrtr->currentRoom->roomNorth != NULL && curCrtr->currentRoom->roomNorth->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curCrtr->currentRoom->roomNorth, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%s %d leaves towards the north\n", curCrtr->typeDesc, curCrtr->creatureID);

            if(curCrtr->type == 1 && curCrtr->currentRoom->roomState == 2)
            {
                cleanRoom(curCrtr->currentRoom, numCrtrs, numRms);
            }

            else if(curCrtr->type == 2 && curCrtr->currentRoom->roomState == 0)
            {
                dirtyRoom(curCrtr->currentRoom, numCrtrs, numRms);
            }
        }

        else
        {
            printf("\nThe north room doesn't exist or is full\n");

            curCrtr->isAngry = true;
            creatureAction(curCrtr);
        }
    }

    else if(strcmp(act, "south") == 0)
    {
        if(curCrtr->currentRoom->roomSouth != NULL && curCrtr->currentRoom->roomSouth->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curCrtr->currentRoom->roomSouth, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%s %d leaves towards the south\n", curCrtr->typeDesc, curCrtr->creatureID);

            if(curCrtr->type == 1 && curCrtr->currentRoom->roomState == 2)
            {
                cleanRoom(curCrtr->currentRoom, numCrtrs, numRms);
            }

            else if(curCrtr->type == 2 && curCrtr->currentRoom->roomState == 0)
            {
                dirtyRoom(curCrtr->currentRoom, numCrtrs, numRms);
            }
        }

        else
        {
            printf("\nThe south room doesn't exist or is full\n");

            curCrtr->isAngry = true;
            creatureAction(curCrtr);
        }
    }

    else if(strcmp(act, "east") == 0)
    {
        if(curCrtr->currentRoom->roomEast != NULL && curCrtr->currentRoom->roomEast->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curCrtr->currentRoom->roomEast, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%s %d leaves towards the east\n", curCrtr->typeDesc, curCrtr->creatureID);

            if(curCrtr->type == 1 && curCrtr->currentRoom->roomState == 2)
            {
                cleanRoom(curCrtr->currentRoom, numCrtrs, numRms);
            }

            else if(curCrtr->type == 2 && curCrtr->currentRoom->roomState == 0)
            {
                dirtyRoom(curCrtr->currentRoom, numCrtrs, numRms);
            }
        }

        else
        {
            printf("\nThe east room doesn't exist or is full\n");

            curCrtr->isAngry = true;
            creatureAction(curCrtr);
        }
    }

    else if(strcmp(act, "west") == 0)
    {
        if(curCrtr->currentRoom->roomWest != NULL && curCrtr->currentRoom->roomWest->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curCrtr->currentRoom->roomWest, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%s %d leaves towards the west\n", curCrtr->typeDesc, curCrtr->creatureID);

            if(curCrtr->type == 1 && curCrtr->currentRoom->roomState == 2)
            {
                cleanRoom(curCrtr->currentRoom, numCrtrs, numRms);
            }

            else if(curCrtr->type == 2 && curCrtr->currentRoom->roomState == 0)
            {
                dirtyRoom(curCrtr->currentRoom, numCrtrs, numRms);
            }
        }

        else
        {
            printf("\nThe west room doesn't exist or is full\n");

            curCrtr->isAngry = true;
            creatureAction(curCrtr);
        }
    }
}

//Moves a creature randomly after it is forced to leave a room
void moveCreatureRandomly(Room* curRm, Creature* curCrtr, int numRms, int numCrtrs)
{
    int randNum = rand() % 4;

    if(randNum == 0)
    {
        if(curRm->roomNorth != NULL && curRm->roomNorth->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomNorth, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the north\n", curCrtr->creatureID);
        }

        else if(curRm->roomSouth != NULL && curRm->roomSouth->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomSouth, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the south\n", curCrtr->creatureID);
        }

        else if(curRm->roomEast != NULL && curRm->roomEast->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomEast, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the east\n", curCrtr->creatureID);
        }

        else if(curRm->roomWest != NULL && curRm->roomWest->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomWest, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the west\n", curCrtr->creatureID);
        }

        else
        {
            killCreature(curRm, curCrtr, numCrtrs);
        }
    }

    else if(randNum == 1)
    {
        if(curRm->roomSouth != NULL && curRm->roomSouth->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomSouth, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the south\n", curCrtr->creatureID);
        }

        else if(curRm->roomEast != NULL && curRm->roomEast->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomEast, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the east\n", curCrtr->creatureID);
        }

        else if(curRm->roomWest != NULL && curRm->roomWest->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomWest, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the west\n", curCrtr->creatureID);
        }

        else if(curRm->roomNorth != NULL && curRm->roomNorth->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomNorth, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the north\n", curCrtr->creatureID);
        }

        else
        {
            killCreature(curRm, curCrtr, numCrtrs);
        }
    }

    else if(randNum == 2)
    {
        if(curRm->roomEast != NULL && curRm->roomEast->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomEast, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the east\n", curCrtr->creatureID);
        }

        else if(curRm->roomWest != NULL && curRm->roomWest->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomWest, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the west\n", curCrtr->creatureID);
        }

        else if(curRm->roomNorth != NULL && curRm->roomNorth->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomNorth, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the north\n", curCrtr->creatureID);
        }

        else if(curRm->roomSouth != NULL && curRm->roomSouth->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomSouth, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the south\n", curCrtr->creatureID);
        }

        else
        {
            killCreature(curRm, curCrtr, numCrtrs);
        }
    }

    else
    {
        if(curRm->roomWest != NULL && curRm->roomWest->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomWest, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the west\n", curCrtr->creatureID);
        }

        else if(curRm->roomNorth != NULL && curRm->roomNorth->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomNorth, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the north\n", curCrtr->creatureID);
        }

        else if(curRm->roomSouth != NULL && curRm->roomSouth->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomSouth, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the south\n", curCrtr->creatureID);
        }

        else if(curRm->roomEast != NULL && curRm->roomEast->creatureCounter < 10)
        {
            curCrtr->currentRoom = moveRooms(numRms, curRm->roomEast, curCrtr->currentRoom);
            curCrtr->currentRoomID = curCrtr->currentRoom->roomID;

            printf("\n%d leaves towards the east\n", curCrtr->creatureID);
        }

        else
        {
            killCreature(curRm, curCrtr, numCrtrs);
        }
    }
}

//Gives the user a list of possible things to do in the game
void showCommands()
{
    printf("\nList of Possible Commands: \n");

    printf("\"look\": Look at the current room's status which involves state, neighbors, and creatures\n");
    printf("\"clean\": Cleans the current room\n");
    printf("\"dirty\": Dirties the current room\n");
    printf("\"north\": You will move to the room north of you, if it exists or if it isn't full\n");
    printf("\"south\": You will move to the room south of you, if it exists or if it isn't full\n");
    printf("\"east\": You will move to the room east of you, if it exists or if it isn't full\n");
    printf("\"west\": You will move to the room west of you, if it exists or if it isn't full\n");
    printf("\"creatureID:command\": You can make the creatures in the current room perform actions by typing the \n");
    printf("creature ID then a colon then the command you want them to perform (EX: 8:north moves creature 8 north \n");
    printf("and 2:clean makes creature 2 clean the current room\n");
    printf("\"exit\": Exits the game\n");
}

//Gives the user a look at the current room properties (ID, State, Neighbors, List of Creatures)
void look(Room* curRm, int numCrtrs)
{
    printf("\nRoom %d, %s, neighbors ", curRm->roomID, curRm->roomDesc);

    if(curRm->roomNorth != NULL)
    {
        printf("%d to the north, ", curRm->roomNorth->roomID);
    }

    if(curRm->roomSouth != NULL)
    {
        printf("%d to the south, ", curRm->roomSouth->roomID);
    }

    if(curRm->roomEast != NULL)
    {
        printf("%d to the east, ", curRm->roomEast->roomID);
    }

    if(curRm->roomWest != NULL)
    {
        printf("%d to the west, ", curRm->roomWest->roomID);
    }

    printf("contains:\n");

    for (int i = 0; i < numCrtrs; i++)
    {
        if(creatures[i].currentRoomID == curRm->roomID)
        {
            if(creatures[i].type == 0)
            {
                printf("%s\n", creatures[i].typeDesc);
            }

            else
            {
                printf("%s %d\n", creatures[i].typeDesc, creatures[i].creatureID);
            }
        }
    }
}

//Cleans the current room and checks to see how creatures react
void cleanRoom(Room* curRm, int numCrtrs, int numRms)
{
    if(curRm->roomState != 0)
    {
        curRm->roomState = (curRm->roomState) - 1;
        getRoomState(curRm);

        for(int i = 0; i < numCrtrs; i++)
        {
            if(creatures[i].type == 2 && creatures[i].currentRoom == curRm && curRm->roomState == 1 && pc->currentRoom == curRm)
            {
                creatures[i].isAngry = true;
                creatureAction(&creatures[i]);
            }

            else if(creatures[i].type == 1 && creatures[i].currentRoom == curRm && curRm->roomState == 1 && pc->currentRoom == curRm)
            {
                creatures[i].isAngry = false;
                creatureAction(&creatures[i]);
            }

            else if(creatures[i].type == 2 && creatures[i].currentRoom == curRm && curRm->roomState == 0 && pc->currentRoom == curRm)
            {
                creatures[i].isAngry = true;
                creatureAction(&creatures[i]);
                moveCreatureRandomly(curRm, &creatures[i], numRms, numCrtrs);

                if(creatures[i].currentRoom != NULL && creatures[i].currentRoom->roomState == 0)
                {
                    dirtyRoom(creatures[i].currentRoom, numCrtrs, numRms, false);
                }
            }

            else if(creatures[i].type == 1 && creatures[i].currentRoom == curRm && curRm->roomState == 0 && pc->currentRoom == curRm)
            {
                creatures[i].isAngry = false;
                creatureAction(&creatures[i]);
            }
        }

        if(pc->currentRoom == curRm)
        {
            printf("\nRoom has been cleaned. Now the state is %s\n", curRm->roomDesc);
        }
    }

    else
    {
        printf("\nRoom cannot be cleaned anymore\n");
    }
}

//Dirties the current room and checks to see how creatures react
void dirtyRoom(Room* curRm, int numCrtrs, int numRms)
{
    if(curRm->roomState != 2)
    {
        curRm->roomState = (curRm->roomState) + 1;
        getRoomState(curRm);

        for(int i = 0; i < numCrtrs; i++)
        {
            if(creatures[i].type == 1 && creatures[i].currentRoom == curRm && curRm->roomState == 1 && pc->currentRoom == curRm)
            {
                creatures[i].isAngry = true;
                creatureAction(&creatures[i]);
            }

            else if(creatures[i].type == 2 && creatures[i].currentRoom == curRm && curRm->roomState == 1 && pc->currentRoom == curRm)
            {
                creatures[i].isAngry = false;
                creatureAction(&creatures[i]);
            }

            else if(creatures[i].type == 1 && creatures[i].currentRoom == curRm && curRm->roomState == 2 && pc->currentRoom == curRm)
            {
                creatures[i].isAngry = true;
                creatureAction(&creatures[i]);
                moveCreatureRandomly(curRm, &creatures[i], numRms, numCrtrs);

                if(creatures[i].currentRoom != NULL && creatures[i].currentRoom->roomState == 2)
                {
                    cleanRoom(creatures[i].currentRoom, numCrtrs, numRms);
                }
            }

            else if(creatures[i].type == 2 && creatures[i].currentRoom == curRm && curRm->roomState == 2 && pc->currentRoom == curRm)
            {
                creatures[i].isAngry = false;
                creatureAction(&creatures[i]);
            }
        }

        if(pc->currentRoom == curRm)
        {
            printf("\nRoom has been dirtied. Now the state is %s\n", curRm->roomDesc);
        }
    }

    else
    {
        printf("\nRoom cannot be dirtied anymore\n");
    }
}

int main()
{
    int numRooms     = 0; //Number of rooms in the world
    int numCreatures = 0; //Number of creatures in the world

    int cID = 0;          //ID of a creature the user controls

    char action[20] = "";     //Command the user chooses
    char* ctrAction   = NULL; //Pointer to the command the user wants a creature to perform

    char endingMessage[255] = ""; //Message displayed when the game ends

    /*************************************************************************************************************************************************/

    printf("Type in your Rooms and Creatures\n");
    printf("(Type Number of Rooms First (Between 1 and 100) followed by \n");
    printf("the room state, north neighbor, south neighbor, east neighbor, west neighbor for each room)\n");
    printf("(Then Type Number of Creatures (Between 1 and 100) followed by \n");
    printf("the creature type and current room for each creature)\n");

    //Retrieves number of rooms in the world (1 < x < 100)
    scanf("%d\n", &numRooms);

    //Allocates memory for the room array and assigns it to a global pointer
    rooms = (Room*) malloc(numRooms * sizeof(Room));
    createRooms(numRooms);

    //Retrieves number of creatures in the world (1 < y < 100)
    scanf("%d\n", &numCreatures);

    //Allocates memory for the creature array and assigns it to a global pointer
    creatures = (Creature*) malloc(numCreatures * sizeof(Creature));
    createCreatures(numCreatures, numRooms);

    findPC(numCreatures);

    printf("\nType help for list of possible commands\n");

    printf("Type a command: \n");
    scanf("%s", action);

    //Continually asks user for input until they exit or win/lose the game
    while(strcmp(action, "exit") != 0)
    {
        if(strcmp(action, "help") == 0)
        {
            showCommands();
        }

        else if(strcmp(action, "look") == 0)
        {
            look(pc->currentRoom, numCreatures);
        }

        else if(strcmp(action, "clean") == 0)
        {
            cleanRoom(pc->currentRoom, numCreatures, numRooms);
        }

        else if(strcmp(action, "dirty") == 0)
        {
            dirtyRoom(pc->currentRoom, numCreatures, numRooms);
        }

        else if(strcmp(action, "north") == 0 || strcmp(action, "south") == 0 || strcmp(action, "east") == 0 || strcmp(action, "west") == 0)
        {
            movePC(numRooms, action);
        }

        //This is for if the PC decides to control a creature in the current room
        else if(strstr(action, ":") != NULL)
        {
            //This looks for the colon in the command and grabs everything before it (Which would be the creature ID) and turns it into an integer
            cID = atoi(strtok(action, ":"));

            //This grabs everything after the colon, but not including the colon
            ctrAction = strtok(NULL, ":");

            if(cID >= numCreatures || creatures[cID].currentRoomID != pc->currentRoomID)
            {
                printf("\nThat creature is not in this room\n");
            }

            else
            {
                creatures[cID].controlFlag = true;

                if(strcmp(ctrAction, "clean") == 0)
                {
                   cleanRoom(creatures[cID].currentRoom, numCreatures, numRooms);
                }

                else if(strcmp(ctrAction, "dirty") == 0)
                {
                    dirtyRoom(creatures[cID].currentRoom, numCreatures, numRooms);
                }

                else if(strcmp(ctrAction, "north") == 0 || strcmp(ctrAction, "south") == 0 || strcmp(ctrAction, "east") == 0 || strcmp(ctrAction, "west") == 0)
                {
                    moveCreature(&creatures[cID], numRooms, numCreatures, ctrAction);
                }
            }

            creatures[cID].controlFlag = false;
        }

        if(respect <= 0)
        {
            strcpy(endingMessage, "\nYour respect is 0 or less. You lose! Try again!\n");
            strcpy(action, "exit");

            break;
        }

        else if(respect >= 80)
        {
            strcpy(endingMessage, "\nYour respect is 80 or more. You win! Good job!\n");
            strcpy(action, "exit");

            break;
        }

        printf("\nType a command: \n");
        scanf("%s", action);
    }

    printf("%s", endingMessage);
    printf("\nGoodbye!\n");

    free(rooms);
    free(creatures);

    return 0;
}
