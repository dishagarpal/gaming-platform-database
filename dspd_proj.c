#include <stdio.h>
#include <string.h>
#define MAX_PLAYERS 100
#define MAX_GAMES 200
#define MAX_GAME_PLAYED 200
#define MAX_GUEST 100

//------------------ structure of the database----------------------

// 1.player information
typedef struct date                //used for registration date.
{
    int day;
    int month;
    int year;
} date;

typedef struct game_details
{
    int total_game_played;          //total games a player has played.
    int total_win;                  //total games the player won.
    int prize;                      //total prize earned
} game_details;

typedef struct player             //main record for a registered player.
{
    int id;
    char name[100];
    date registration_date;        //nested dates struct.
    game_details game_played;       //nested game_details struct
    int game_preferences[3];       

} player;

// 2.game master details
typedef struct game_master          //data about each game offered by the platform.
{
    int game_id;                    //unique id for the game.
    char name[100];                  //name for the game
    char type[100];                  //multiplayer or singleplayer
    int no_of_player;               //maximum players allowed

} game_master;

// 3. game played details
typedef struct game_played_details
{
    int game_id;                    //which game was played
    int no_of_player;               //actual number of players in this session
    int player_id[4];               // maximiumn player in game is 4;
    int player_score[4];            //corresponding scores for each player_id.
    int winner_id;                  //the id of the player who won (should match one of player_id[]
    int player_active_days[4];      //number of days each player has been active
    int days;                       //total days the game session spanned

} play_game;

// 4. guest users details
typedef struct guest_users
{
    int id;
    char name[100];
    int days;

} guest_users;

//------------ function of the database-----------------
void save_initial_data(player p[], game_master gm[], play_game g[]) {
    
    FILE *fp;

    // ---------------- SAVE PLAYERS ----------------
    fp = fopen("players.txt", "w");
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (p[i].id == 0) continue;
        fprintf(fp, "%d,%s,%02d/%02d/%04d,%d,%d,%d,%d,%d,%d\n",
                p[i].id, p[i].name,
                p[i].registration_date.day,
                p[i].registration_date.month,
                p[i].registration_date.year,
                p[i].game_played.total_game_played,
                p[i].game_played.total_win,
                p[i].game_played.prize,
                p[i].game_preferences[0],
                p[i].game_preferences[1],
                p[i].game_preferences[2]);
    }
    fclose(fp);

    // ---------------- SAVE GAMES ----------------
    fp = fopen("games.txt", "w");
    for (int i = 0; i < MAX_GAMES; i++) {
        if (gm[i].game_id == 0) continue;
        fprintf(fp, "%d,%s,%s,%d\n",
                gm[i].game_id, gm[i].name,
                gm[i].type, gm[i].no_of_player);
    }
    fclose(fp);

    // ---------------- SAVE GAMEPLAY ----------------
    fp = fopen("gameplay.txt", "w");
    for (int i = 0; i < MAX_GAME_PLAYED; i++) {
        if (g[i].game_id == 0) continue;

        fprintf(fp, "%d,%d,", g[i].game_id, g[i].no_of_player);

        for (int j = 0; j < 4; j++) fprintf(fp, "%d,", g[i].player_id[j]);
        for (int j = 0; j < 4; j++) fprintf(fp, "%d,", g[i].player_score[j]);
        fprintf(fp, "%d,", g[i].winner_id);
        for (int j = 0; j < 4; j++) fprintf(fp, "%d,", g[i].player_active_days[j]);

        fprintf(fp, "%d\n", g[i].days);
    }
    fclose(fp);

    printf("Initial data saved into files.\n");
}

//...
void load_players(player p[]) {
    FILE *fp = fopen("players.txt", "r");
    if (!fp) return;

    int idx = 0;
    char date_str[20];

    while (fscanf(fp, "%d,%[^,],%[^,],%d,%d,%d,%d,%d,%d\n",
                  &p[idx].id,
                  p[idx].name,
                  date_str,
                  &p[idx].game_played.total_game_played,
                  &p[idx].game_played.total_win,
                  &p[idx].game_played.prize,
                  &p[idx].game_preferences[0],
                  &p[idx].game_preferences[1],
                  &p[idx].game_preferences[2]) == 9) {

        sscanf(date_str, "%d/%d/%d",
               &p[idx].registration_date.day,
               &p[idx].registration_date.month,
               &p[idx].registration_date.year);

        idx++;
    }
    fclose(fp);
}

void save_new_player(player p) {
    FILE *fp = fopen("players.txt", "a");

    fprintf(fp,
        "%d,%s,%02d/%02d/%04d,%d,%d,%d,%d,%d,%d\n",
        p.id, p.name,
        p.registration_date.day,
        p.registration_date.month,
        p.registration_date.year,
        p.game_played.total_game_played,
        p.game_played.total_win,
        p.game_played.prize,
        p.game_preferences[0],
        p.game_preferences[1],
        p.game_preferences[2]
    );

    fclose(fp);
}
 void rewrite_all_players(player p[]) {
    FILE *fp = fopen("players.txt", "w");
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (p[i].id == 0) continue;

        fprintf(fp, "%d,%s,%02d/%02d/%04d,%d,%d,%d,%d,%d,%d\n",
                p[i].id, p[i].name,
                p[i].registration_date.day,
                p[i].registration_date.month,
                p[i].registration_date.year,
                p[i].game_played.total_game_played,
                p[i].game_played.total_win,
                p[i].game_played.prize,
                p[i].game_preferences[0],
                p[i].game_preferences[1],
                p[i].game_preferences[2]);
    }
    fclose(fp);
}




// --------------------------------------------------- 1.add function ----------------------------------------
//players p[]: array of player records ,the function will add new players here.
//game_master game[]: array of available games,used to show the list of games and their IDs/names.
//play_game game_played[]: array containing past game sessions ,used to calculate which games are “most played.”

void add(player p[], game_master game[], play_game game_played[])
{

    int next_index = 0;                 //will store the index in array p[] where the next player’s data should go
    int next_id = 0;                    //will store the new player’s ID


    int n;
    printf("enter the number of the player you want to take the input\n");
    scanf("%d", &n);
    int ind = 0;                        //initialized to 0 used to find the next available empty player slot.
    for (int j = 0; j < n; j++)         //runs n times ,once per player to be added
    {

        while (p[ind].id != 0)          //skip filled slots
        {
            ind++;
        }

        next_index = ind;
        if (ind == 0)
        {
            next_id = 101;
        }
        else
        {
            next_id = p[ind - 1].id + 1;    //new id is the previous player’s id + 1.
        }

        p[next_index].id = next_id;         //assign this id to the new player record.

        printf("Enter the name of the player:-\n");
        scanf(" %[^\n]", p[next_index].name);

        char input[20];
        printf("Enter date (dd/mm/yyyy): ");
        scanf("%s", input);
        sscanf(input, "%d/%d/%d", &p[next_index].registration_date.day, &p[next_index].registration_date.month, &p[next_index].registration_date.year);

        printf("Select your game prefrences(any 3):-\n");

        //-----------------------------------------print available games---------------------------------------------------------- 


        int game_count = 0;
        for (int i = 0; i < MAX_GAMES; i++)
        {
            if(game[i].game_id==0) continue;    
            printf("%d. %s\n", game[i].game_id, game[i].name);
            game_count++;           //Counts valid games in game_count
        }
        

        //------------------------------------------compute most-played and second-most-played game ids using game_played-------------------


        int hasharr[MAX_GAME_PLAYED] = {0};         
        for (int i = 0; i < MAX_GAME_PLAYED; i++)
        {
            if(game_played[i].game_id==0) continue;
            int gid = game_played[i].game_id;
            if (gid >= 1 && gid <= 200)
            {
                hasharr[gid]++;
            }
        }

        int max1 = 0, id1 = 0, max2 = 0, id2 = 0;
        for (int i = 1; i <= 100; i++)
        {
            if (hasharr[i] > max1)
            {
                max2 = max1;
                id2 = id1;
                max1 = hasharr[i];
                id1 = i;
            }
            else if (hasharr[i] > max2)
            {
                max2 = hasharr[i];
                id2 = i;
            }
        }

        if (id1 != 0 && id1 - 1 < 200)
        {
            printf("\n\n\nmost played game is id: %d. %s\n", id1, game[id1 - 1].name);
        }
        else{
            printf("\n\n\nmost played game data not available\n");
        }
        if (id2 != 0 && id2 - 1 < 200)
        {
            printf("2nd most played game is id: %d. %s\n\n", id2, game[id2 - 1].name);
        }
        else{
            printf("2nd most played game data not available\n\n");
        }

   
        for (int m = 0; m < 3; m++)
        {
            int k ;
            //game_count is total number of available games printed earlier in the program.
            printf("choose %d preference (enter id 1-%d):\n", m + 1, game_count);
            scanf("%d", &k);
            p[next_index].game_preferences[m] = game[k - 1].game_id; //The array of games is zero-indexed

        }

        printf("enter the total game played by the player:-\n");
        scanf("%d", &p[next_index].game_played.total_game_played);
        printf("enter the total win of the player:-\n");
        scanf("%d", &p[next_index].game_played.total_win);
        printf("enter the prize won by the player:-\n");
        scanf("%d", &p[next_index].game_played.prize);

        //----------------------------------------------- displaying added player information------------------------------------------------------------

        printf("\nplayer added successfully with id:%d\n", p[next_index].id);
        // printf("Name: %s\n", p[next_index].name);
        // printf("Registration Date: %02d/%02d/%04d\n", p[next_index].registration_date.day, p[next_index].registration_date.month, p[next_index].registration_date.year);
        // printf("Game Preferences: ");
        // for (int m = 0; m < 3; m++)
        // {
        //     printf("%d, ", p[next_index].game_preferences[m]);
        // }
        // printf("\nTotal Games Played: %d\n", p[next_index].game_played.total_game_played);
        // printf("Total Wins: %d\n", p[next_index].game_played.total_win);
        // printf("Prize Won: %d\n", p[next_index].game_played.prize);
        save_new_player(p[next_index]);




    

    


    //-----------------------------------file handling------------------------------------------------------------------------------------


        FILE *fp = fopen("players.txt", "a");

        if(fp==NULL){
            printf("file is having some error in appending data");
        }
        else{
            fprintf(fp, "%d,%s,%d/%d/%d,%d,%d,%d,%d,%d,%d\n",

                        

                        p[next_index].id,
                        p[next_index].name,
                        p[next_index].registration_date.day,
                        p[next_index].registration_date.month,
                        p[next_index].registration_date.year,
                        p[next_index].game_played.total_game_played,
                        p[next_index].game_played.total_win,
                        p[next_index].game_played.prize,
                        p[next_index].game_preferences[0],
                        p[next_index].game_preferences[1],
                        p[next_index].game_preferences[2] ) ;

                fclose(fp);
                printf("New player saved to 'players.txt'\n");

        }



        ind++;
    }
}

//--------------------------------------------------update the game details--------------------------------------------

void update(player player[], guest_users guest[] ){
    int id;
    
    printf("enter the id of the player:-\n");
    scanf("%d", &id);
    printf("enter the total game of the player:-");
    scanf("%d", &player[id-101].game_played.total_game_played);
    printf("enter the winning game:-");
    scanf("%d", &player[id-101].game_played.total_win);
    printf("Enter the prize:-");
    scanf("%d", &player[id-101].game_played.prize);

    printf("\n information updated successfully\n");
    rewrite_all_players(player);

    
    


}

//-----------------------search operation--------------------------------

void game_info(player player[], play_game game[], game_master gm[])
{
    int player_id;
    printf("enter the id of the player:-\n");
    scanf("%d", &player_id);
   

    int games_found[100]; // id of the games played by this users
    int game_count = 0;     //tracks how many distinct games were found

    for (int i = 0; i < MAX_GAME_PLAYED; i++)       //Iterate each entry in the game[] array
    {
        if (game[i].game_id == 0) continue;    
        for (int j = 0; j < game[i].no_of_player; j++)  //Iterate the players in this session (0..no_of_player-1)
        {
            if (game[i].player_id[j] == player_id)
            {
                
                games_found[game_count++] = game[i].game_id;
                break; 
            }
        }
    }

    if (game_count == 0){
    
        printf("\nThis player has not played the any game...\n");
        return;
    }
    else{
        printf("These are the games played by the users\n");
        for(int i=0; i<game_count; i++){
            printf("Game ID: %d - %s\n", games_found[i], gm[games_found[i]-1].name);
        }
    }

    int chosen_game;
    printf("\nEnter the game id to see who else has played it: ");
    scanf("%d", &chosen_game);
    printf("\nOther playeres who played the game of ID %d:\n", chosen_game);

    int found_any = 0;

    for (int i = 0;  i < MAX_GAME_PLAYED; i++)
    {

        if (game[i].game_id == 0) continue;
        if (game[i].game_id == chosen_game)
        {
            for (int j = 0; j < game[i].no_of_player; j++)
            {
                int pid = game[i].player_id[j];
                if (pid != player_id)
                { 
                    // find player name from players............
                    for (int k = 0; k < 101; k++)
                    {
                        if(player[k].id == 0) continue;
                        if (player[k].id == pid)
                        {
                            printf("Player Name: %s (ID: %d)\n", player[k].name, player[k].id);
                            found_any = 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    if (!found_any)
    {
        printf("No other players found for this game.\n");
    }
}


//----------------------------------------------2.2------------------------------------------------------
void inactive_user(player player[]){
    printf("-----------------INACTIVE PLAYERS-----------------");
    for(int i=0;  i<MAX_PLAYERS; i++){
        if(player[i].id == 0) continue;
        if(player[i].game_played.total_game_played==0){
            printf("this users has not played the game yet  id:%d-%s\n", player[i].id, player[i].name);
        }
    }


}

//------------------------------------------2.3-------------------------------------------------------------

void active_user(player player[]){
    int k;
    printf("Enter the threshold game limit:-\n");
    scanf("%d", &k);
    for(int i=0; i<MAX_PLAYERS; i++){
        if(player[i].id == 0) continue;
        if(player[i].game_played.total_game_played>k){
            printf("Active user, id:%d-%s\n", player[i].id, player[i].name);
        }
    }
}



//-----------------------------------3.1--------------------------------------------------------------------


void most_active_game(player player[], play_game game[], game_master gm[]){
    int game_play_count[MAX_GAME_PLAYED]  = {0}; 

    // count how many times each game has been played------------------------------------------------------------
    for (int i = 0; i < MAX_GAME_PLAYED; i++) 
    {
        if(game[i].game_id==0) continue;
        game_play_count[game[i].game_id]++;
    }

    // find the game which has maximum game count------------------------------------------------------------
    int max_count = 0;
    int most_active_game_id = -1;
    for (int i = 1; i <= 100; i++) {
        if (game_play_count[i] > max_count) {
            max_count = game_play_count[i];
            most_active_game_id = i;
        }
    }

    if (most_active_game_id != -1) {
        printf("most active game is: %s (ID: %d) with %d plays\n", 
               gm[most_active_game_id - 1].name, 
               most_active_game_id, 
               max_count);
    } else {
        printf("No games have been played yet.\n");
    }
}



//---------------------------------------3.2--------------------------------------------------------------------



void top_5_games_by_total_days(play_game game[], game_master gm[]) {
    int total_days[201] = {0};      

    // adding the days--------------------------------------------------------------------------------
    for (int i = 0; i < MAX_GAME_PLAYED; i++) {
        if (game[i].game_id == 0) continue;
        total_days[game[i].game_id] += game[i].days;
    }

    //  search for the top 5 game-----------------------------------------------------------------------
     for (int c = 0; c < 5; c++) {
        int max_days = 0, max_id = 0;
        for (int i = 1; i <= 200; i++) {
            if (total_days[i] > max_days) {
                max_days = total_days[i];
                max_id = i;
            }
        }

        if (max_id == 0) {
            break;
        } 


        for (int g = 0; g < MAX_GAMES; g++) {
            if (gm[g].game_id == 0) continue;
            if (gm[g].game_id == max_id) {
                printf("%d. Game ID: %d | Name: %s | Type: %s | Total Days: %d\n",
                       c + 1, max_id, gm[g].name, gm[g].type, max_days);
                break;
            }
        }

        total_days[max_id] = 0;                //again assigning to the zero..so not to repeat--
    }
}





//----------------------------------3.3---------------------------------------------------------------
void players_currentplaying(game_master gm[], play_game game_played[], player player[]){
    int gid;
    for(int i=0; i<MAX_GAMES; i++){
        if(gm[i].game_id == 0) continue;
        printf("id: %d  - %s\n", gm[i].game_id, gm[i].name);

    }
    printf("choose the game id whose current player u want to see:\n");
    scanf("%d", &gid);

    int pid[50]={0};
    int score[50]={0};
    int count=0;
    for(int i=0; i<MAX_GAME_PLAYED; i++){
        if(game_played[i].game_id == 0) continue;
        if(game_played[i].game_id==gid){
            for(int j=0; j<game_played[i].no_of_player; j++){
                
                pid[count]=game_played[i].player_id[j];
                score[count]=game_played[i].player_score[j];
                count++;
            }
        }
    }

    if(count==0){
        printf("no players are currently playing this game:\n");
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (score[i] < score[j]) {
                int temp = score[i];
                score[i] = score[j];
                score[j] = temp;

                int tempid = pid[i];
                pid[i] = pid[j];
                pid[j] = tempid;
            }
        }
    }

    printf("players currently playing game are:-\n");

    for (int i = 0; i < count; i++) {
        printf("[player_name : %s,\tscore : %d,\tprize : %d,\tid : %d\t]\n",
        player[pid[i]-101].name,score[i], player[pid[i]-101].game_played.prize, pid[i]); 
    }

}

//----------------------------------------------------4------------------------------------------------------

void delete_guest(guest_users guest[]){
    for(int i=0; i<MAX_GUEST; i++){
        if(guest[i].id==0)continue;

        if(guest[i].days>20){   
            printf("deleting guest user id:%d - %s\n", guest[i].id, guest[i].name);
            guest[i].id=0;
            guest[i].days=0;
            guest[i].name[0]='\0';
        }
    }



}


//------------------------------------------load the added player back to the array for the next time------------

void load_player(player p[]) {
    int index = 13; 

    FILE *fp2 = fopen("players.txt", "r");
    if (fp2 == NULL) {
        printf("player.txt file do not exist:\n");
        return;
    }

    char date_str[20];
    while (fscanf(fp2, "%d,%[^,],%[^,],%d,%d,%d,%d,%d,%d\n",
                  &p[index].id,
                  p[index].name,
                  date_str,
                  &p[index].game_played.total_game_played,
                  &p[index].game_played.total_win,
                  &p[index].game_played.prize,
                  &p[index].game_preferences[0],
                  &p[index].game_preferences[1],
                  &p[index].game_preferences[2]) == 9) {

        sscanf(date_str, "%d/%d/%d",
               &p[index].registration_date.day,
               &p[index].registration_date.month,
               &p[index].registration_date.year);

        index++;
    }

    fclose(fp2);
    // printf("players are loded back to the memory:\n");
}





//--------------------main function-----------------------------------------------------------
int main()
{
 
    
    // stored players information.. 

    player player[MAX_PLAYERS] = {
        {101, "Prince", {1, 11, 2025}, {4, 0, 400}, {1,2,3}},
        {102, "Sam", {15, 11, 2025}, {1, 1, 100}, {1,4,3}},
        {103, "Rohit", {12, 11, 2025}, {4, 2, 300}, {5,6,7}},
        {104, "Aditi", {18, 11, 2025}, {3, 1, 200}, {2, 8, 9}},          
        {105, "Vikram", {20, 11, 2025}, {2, 0, 50}, {7, 3, 10}},      
        {106, "Tina", {22, 11, 2025}, {2, 1, 150}, {4, 1, 19}},          
        {107, "Aryan", {25, 11, 2025}, {1, 1, 100}, {6, 17, 2}},         
        {108, "Nisha", {27, 11, 2025}, {2, 2, 200}, {5, 16, 14}},        
        {109, "Ravi", {28, 11, 2025}, {0, 0, 0}, {2, 1, 3}}, 
        {110, "Sneha", {29, 11, 2025}, {0, 0, 0}, {10, 14, 19}},         
        {111, "Karan", {30, 11, 2025}, {0, 0, 0}, {5, 6, 8}},            
        {112, "Meera", {1, 12, 2025}, {0, 0, 0}, {2, 16, 9}},           
        {113, "Dev", {2, 12, 2025}, {0, 0, 0}, {1, 10, 3}} 
    };
       
    // stored game master information..

    game_master game[MAX_GAMES] = {
        {1, "chess", "multiuser", 2},
        {2, "ludo", "multiuser", 4},
        {3, "carrom", "multiuser", 4},
        {4, "clashofclan", "singleuser", 1},
        {5, "cricketclash", "singleuser", 1},
        {6, "footballstrike", "singleuser", 1},
        {7, "carddeck", "multiuser", 4},
        {8, "tabletennis", "multiuser", 2},
        {9, "badmintonduel", "multiuser", 2},
        {10, "poolchamp", "multiuser", 2},
        {11, "racingfever", "singleuser", 1},
        {12, "sniperzone", "singleuser", 1},
        {13, "mysteryhunt", "singleuser", 1},
        {14, "battlearena", "multiuser", 4},
        {15, "minigolftour", "multiuser", 2},
        {16, "archeryking", "singleuser", 1},
        {17, "dragrace", "multiuser", 2},
        {18, "wordconnect", "singleuser", 1},
        {19, "quizmaster", "multiuser", 4},
        {20, "spaceinvader", "singleuser", 1}
    };

    play_game game_played[MAX_GAME_PLAYED] = {
        {1, 2, {101, 102}, {90, 95}, 102, {20, 22}, 25},
        {1, 2, {103, 104}, {85, 80}, 103, {21, 19}, 30},
        {1, 2, {105, 106}, {78, 90}, 106, {18, 22}, 28},
        {4, 1, {107}, {100}, 107, {25}, 28},
        {4, 1, {108}, {120}, 108, {27}, 29},
        {6, 2, {101, 103}, {88, 92}, 103, {19, 21}, 24},
        {7, 2, {104, 105}, {100, 94}, 104, {20, 19}, 23},
        {10, 2, {106, 108}, {89, 95}, 108, {22, 23}, 27},        
        {11, 2, {109, 110}, {75, 80}, 110, {15, 18}, 26},
        {12, 1, {111}, {85}, 111, {12},    22},
        {13, 1, {112}, {90}, 112, {14}, 18},
        {14, 4, {113, 101, 102, 103}, {70, 80, 75, 85}, 103, {20, 22, 21, 23}, 30}
    };
    save_initial_data(player, game, game_played);
    load_players(player);


   

    guest_users guest[MAX_GUEST] = {
        {201, "guest1", 5},
        {202, "guest2", 3},
        {203, "guest3", 10},
        {204, "guest4", 2},
        {205, "guest5", 15},
        {206, "guest6", 6},
        {207, "guest7", 19},
        {208, "guest8", 8},
        {209, "guest9", 24},
        {210, "guest10", 23}};

       

        
        int choice;
        load_player(player);
    do {
    printf("\n================= GAME DATABASE MENU =================\n");
    printf("1.  Add New Player\n");
    printf("2.  Update Player Game Details\n");
    printf("3.  Show Player's Game Info\n");
    printf("4.  Show Inactive Users (never played)\n");
    printf("5.  Show Active Users (above threshold)\n");
    printf("6.  Show Most Active Game (by play count)\n");
    printf("7.  Show Top 5 Games by Total Days\n");
    printf("8.  Show Players Currently Playing a Game (sorted by score)\n");
    printf("9.  Delete Guest Users (Days > 20)\n");
    printf("10. Exit\n");
    printf("------------------------------------------------------\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            add(player, game, game_played);
            break;

        case 2:
            update(player, guest);
            break;

        case 3:
            game_info(player, game_played, game);
            break;

        case 4:
            inactive_user(player);
            break;

        case 5:
            active_user(player);
            break;

        case 6:
            most_active_game(player, game_played, game);
            break;

        case 7:
            top_5_games_by_total_days(game_played, game);
            break;

        case 8:
            players_currentplaying(game, game_played, player);
            break;

        case 9:
            delete_guest(guest);
            break;

        case 10:
            printf("Exiting the system... Thank you!\n");
            break;

        default:
            printf("Invalid choice! Please try again.\n");
            break;
    }

    
} while (choice != 10);



    return 0;
}