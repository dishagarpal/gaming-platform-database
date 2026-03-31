#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 100
#define MAX_GAMES 200
#define MAX_GAME_PLAYED 200
#define MAX_GUEST 100


typedef struct date {
    int day;
    int month;
    int year;
} date;

typedef struct game_details {
    int total_game_played;
    int total_win;
    int prize;
} game_details;

typedef struct player {
    int id;
    char name[100];
    date registration_date;
    game_details game_played;
    int game_preferences[3];
} player;

typedef struct game_master {
    int game_id;
    char name[100];
    char type[100];
    int no_of_player;
} game_master;

typedef struct game_played_details {
    int game_id;
    int no_of_player;
    int player_id[4];
    int player_score[4];
    int winner_id;
    int player_active_days[4];
    int days;
} play_game;

typedef struct guest_users {
    int id;
    char name[100];
    int days;
} guest_users;


void save_players(player p[]);
void load_players(player p[]);
void save_games(game_master g[]);
void load_games(game_master g[]);
void save_game_played(play_game gp[]);
void load_game_played(play_game gp[]);
void save_guests(guest_users gs[]);
void load_guests(guest_users gs[]);


int first_free_player_index(player p[]) {
    for (int i = 0; i < MAX_PLAYERS; ++i) 
    
    {
        if (p[i].id == 0) 
        {
            return i;
        }
    };
    return -1;
}


void add(player p[], game_master game[], play_game game_played[]) {
   int n;
    printf("Enter number of players to add: ");
    scanf("%d", &n);
    while (getchar() != '\n');  


    for (int j = 0; j < n; ++j) {
        int ind = first_free_player_index(p);
        if (ind == -1) 
        { printf("Player array full.\n");
             return;
        }

        int next_id;
        if (ind == 0) 
        {
            next_id = 101;
        }
        else
        {
            int k = ind - 1;
            while (k >= 0 && p[k].id == 0) 
            {--k;}
             if (k >= 0) 
            {
            next_id = p[k].id + 1;
            } else 
            {
             next_id = 101;
            }

        }
        p[ind].id = next_id;
        printf("Enter the name of the player/username:-\n");
        scanf(" %[^\n]", p[ind].name);

        char input[20];
        printf("Enter date (dd/mm/yyyy): ");
        scanf("%s", input);
        sscanf(input, "%d/%d/%d", &p[ind].registration_date.day, &p[ind].registration_date.month, &p[ind].registration_date.year);

        printf("Select your game prefrences(any 3):-\n");

        // print games
        int game_count = 0;
        for (int i = 0; i < MAX_GAMES; ++i) {
            if (game[i].game_id == 0) continue;
            printf("%d. %s\n", game[i].game_id, game[i].name);
            game_count++;
        }
        // compute most played & 2nd most played
        int hasharr[MAX_GAME_PLAYED] = {0};           //array to count how many times each game was played
        for (int i = 0; i < MAX_GAME_PLAYED; ++i) {
            if (game_played[i].game_id == 0) continue;
            int gid = game_played[i].game_id;
            if (gid >= 1 && gid <= 200) hasharr[gid]++;
        }
        int max1=0,id1=0,max2=0,id2=0;
        for (int i=1;i<=100;i++){
            if (hasharr[i] > max1) 
            { max2=max1; id2=id1; max1=hasharr[i]; id1=i; }
            else if (hasharr[i] > max2)
             { max2 = hasharr[i]; id2 = i; }
        }
        if (id1 != 0 && id1 - 1 < MAX_GAMES) 
        printf("\nMost played game: %d . %s\n", id1, game[id1-1].name);
        else 
        printf("\nMost played game data not available\n");
        if (id2 != 0 && id2 - 1 < MAX_GAMES)
        printf("2nd most played: %d . %s\n", id2, game[id2-1].name);
        else 
        printf("2nd most played game data not available\n");

        for (int m = 0; m < 3; ++m) {
            int k;
            printf("Choose %d preference (enter id 1-%d): ", m+1, game_count);
            if (scanf("%d", &k) != 1) { p[ind].game_preferences[m] = 0; }
            else p[ind].game_preferences[m] = (k - 1 >= 0 && k-1 < MAX_GAMES) ? game[k-1].game_id : 0;
        }
        printf("Enter total games played: ");
        scanf("%d", &p[ind].game_played.total_game_played);
        printf("Enter total wins: ");
        scanf("%d", &p[ind].game_played.total_win);
        printf("Enter prize won: ");
        scanf("%d", &p[ind].game_played.prize);

        printf("\nPlayer added successfully with id: %d\nName: %s\nRegistration Date: %02d/%02d/%04d\nGame Preferences: %d, %d, %d\nTotal Games Played: %d\nTotal Wins: %d\nPrize Won: %d\n",
               p[ind].id, p[ind].name, p[ind].registration_date.day, p[ind].registration_date.month, p[ind].registration_date.year,
               p[ind].game_preferences[0], p[ind].game_preferences[1], p[ind].game_preferences[2],
               p[ind].game_played.total_game_played, p[ind].game_played.total_win, p[ind].game_played.prize);
        save_players(p);
    }
}

void update(player player_arr[], guest_users guest[]) {
    int id;
    printf("Enter the id of the player: ");
    if (scanf("%d", &id) != 1) return;
    int idx = -1;
    for (int i = 0; i < MAX_PLAYERS; ++i) 
    {
        if (player_arr[i].id == id) 
        { idx = i; break; }
    }
    if (idx == -1)
     {  printf("Player id not found.\n"); 
        return;
     }

    printf("Enter the total game of the player: ");
    scanf("%d", &player_arr[idx].game_played.total_game_played);
    printf("Enter the winning games: ");
    scanf("%d", &player_arr[idx].game_played.total_win);
    printf("Enter the prize: ");
    scanf("%d", &player_arr[idx].game_played.prize);

    printf("\nInformation updated successfully\n");
    save_players(player_arr);
}

void save_players(player p[]) {
    FILE *fp = fopen("players.txt", "w");
    if (!fp)
    { printf("Error opening players.txt for writing\n"); return; }
    for (int i = 0; i < MAX_PLAYERS; ++i) {
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
    printf("Saved players to players.txt\n");
}

void load_players(player p[]) {
    FILE *fp = fopen("players.txt", "r");
    if (!fp) {
               return;
    }
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        int id, d,m,y, tg, tw, pr, g0,g1,g2;
        char name[100];
        if (sscanf(line, "%d,%100[^,],%d/%d/%d,%d,%d,%d,%d,%d,%d",
                   &id, name, &d, &m, &y, &tg, &tw, &pr, &g0, &g1, &g2) >= 1) {
            int idx = first_free_player_index(p);
            if (idx == -1) break;
            p[idx].id = id;
            strncpy(p[idx].name, name, sizeof(p[idx].name)-1);
            p[idx].name[sizeof(p[idx].name)-1] = '\0';
            p[idx].registration_date.day = d;
            p[idx].registration_date.month = m;
            p[idx].registration_date.year = y;
            p[idx].game_played.total_game_played = tg;
            p[idx].game_played.total_win = tw;
            p[idx].game_played.prize = pr;
            p[idx].game_preferences[0] = g0;
            p[idx].game_preferences[1] = g1;
            p[idx].game_preferences[2] = g2;
        }
    }
    fclose(fp);
    printf("Loaded players from players.txt (if present)\n");
}

void save_games(game_master g[]) {
    FILE *fp = fopen("games.txt", "w");
    if (!fp) { printf("Error opening games.txt for writing\n"); return; }
    for (int i = 0; i < MAX_GAMES; ++i){
        if (g[i].game_id == 0) continue;
        fprintf(fp, "%d,%s,%s,%d\n", g[i].game_id, g[i].name, g[i].type, g[i].no_of_player);
    }
    fclose(fp);
    printf("Saved games to games.txt\n");
}

void load_games(game_master g[]) {
    FILE *fp = fopen("games.txt", "r");
    if (!fp) return;
    char line[256];
    int index = 0;
    while (fgets(line, sizeof(line), fp)) {
        int id, nop;
        char name[100], type[100];
        if (sscanf(line, "%d,%100[^,],%100[^,],%d", &id, name, type, &nop) >= 1) {
            // place into first free slot
            int i;
            for (i = 0; i < MAX_GAMES; ++i) if (g[i].game_id == 0) break;
            if (i == MAX_GAMES) break;
            g[i].game_id = id;
            strncpy(g[i].name, name, sizeof(g[i].name)-1); g[i].name[sizeof(g[i].name)-1] = '\0';
            strncpy(g[i].type, type, sizeof(g[i].type)-1); g[i].type[sizeof(g[i].type)-1] = '\0';
            g[i].no_of_player = nop;
        }
    }
    fclose(fp);
    printf("Loaded games from games.txt .\n");
}

void save_game_played(play_game gp[]) {
    FILE *fp = fopen("game_played.txt", "w");
    if (!fp) { printf("Error opening game_played.txt for writing\n"); return; }
    for (int i = 0; i < MAX_GAME_PLAYED; ++i) {
        if (gp[i].game_id == 0) continue;
        fprintf(fp, "%d,%d,", gp[i].game_id, gp[i].no_of_player);
        for (int j = 0; j < gp[i].no_of_player; ++j) {
            fprintf(fp, "%d:", gp[i].player_id[j]);
        }
        fprintf(fp, ",");
        for (int j = 0; j < gp[i].no_of_player; ++j) {
            fprintf(fp, "%d:", gp[i].player_score[j]);
        }
        fprintf(fp, ",%d,", gp[i].winner_id);
        for (int j = 0; j < gp[i].no_of_player; ++j) {
            fprintf(fp, "%d:", gp[i].player_active_days[j]);
        }
        fprintf(fp, ",%d\n", gp[i].days);
    }
    fclose(fp);
    printf("Saved game_played to game_played.txt\n");
}

void load_game_played(play_game gp[])
{
    FILE *fp = fopen("game_played.txt", "r");
    if (!fp) return;

    int gid, nop, winner, days;
    int p1, p2, p3, p4;
    int s1, s2, s3, s4;
    int d1, d2, d3, d4;

    while (fscanf(fp,
        "%d,%d,%d:%d:%d:%d,%d:%d:%d:%d,%d,%d:%d:%d:%d,%d",
        &gid, &nop,
        &p1, &p2, &p3, &p4,
        &s1, &s2, &s3, &s4,
        &winner,
        &d1, &d2, &d3, &d4,
        &days
    ) == 15)
    {
        int i;
        for (i = 0; i < MAX_GAME_PLAYED; i++)
            if (gp[i].game_id == 0) break;

        if (i == MAX_GAME_PLAYED) break;

        gp[i].game_id = gid;
        gp[i].no_of_player = nop;
        gp[i].winner_id = winner;
        gp[i].days = days;

        gp[i].player_id[0] = p1;
        gp[i].player_id[1] = p2;
        gp[i].player_id[2] = p3;
        gp[i].player_id[3] = p4;

        gp[i].player_score[0] = s1;
        gp[i].player_score[1] = s2;
        gp[i].player_score[2] = s3;
        gp[i].player_score[3] = s4;

        gp[i].player_active_days[0] = d1;
        gp[i].player_active_days[1] = d2;
        gp[i].player_active_days[2] = d3;
        gp[i].player_active_days[3] = d4;
    }

    fclose(fp);

    printf("Loaded game_played from game_played.txt\n");
}


void save_guests(guest_users gs[]) {
    FILE *fp = fopen("guests.txt", "w");
    if (!fp) { printf("Error opening guests.txt for writing\n"); return; }
    for (int i = 0; i < MAX_GUEST; ++i) {
        if (gs[i].id == 0) continue;
        fprintf(fp, "%d,%s,%d\n", gs[i].id, gs[i].name, gs[i].days);
    }
    fclose(fp);
    printf("Saved guests to guests.txt\n");
}

void load_guests(guest_users gs[]) {
    FILE *fp = fopen("guests.txt", "r");
    if (!fp) return;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int id, days; char name[100];
        if (sscanf(line, "%d,%100[^,],%d", &id, name, &days) >= 2) {
            int i;
            for (i = 0; i < MAX_GUEST; ++i) 
            if (gs[i].id == 0) break;
            if (i == MAX_GUEST) break;
            gs[i].id = id;
            strncpy(gs[i].name, name, sizeof(gs[i].name)-1);
            gs[i].name[sizeof(gs[i].name)-1] = '\0';
            gs[i].days = days;
        }
    }
    fclose(fp);
    printf("Loaded guests from guests.txt (if present)\n");
}

void game_info(player player_arr[], play_game game[], game_master gm[]) {
    int player_id;
    printf("Enter the id of the player: ");
    if (scanf("%d", &player_id) != 1) return;
    int games_found[100]; int game_count=0;
    for (int i=0;i<MAX_GAME_PLAYED;i++){
        if (game[i].game_id==0) continue;
        for (int j=0;j<game[i].no_of_player;j++){
            if (game[i].player_id[j]==player_id){
                games_found[game_count++]=game[i].game_id;
                break;
            }
        }
    }
    if (game_count==0) { printf("\nThis player has not played any game...\n"); return; }
    printf("These are the games played by the user:\n");
    for (int i=0;i<game_count;i++) {
        printf("Game ID: %d - %s\n", games_found[i], gm[games_found[i]-1].name);
    }
    int chosen_game;
    printf("\nEnter the game id to see who else has played it: ");
    if (scanf("%d", &chosen_game) != 1) return;
    printf("\nOther players who played the game of ID %d:\n", chosen_game);
    int found_any = 0;
    for (int i=0;i<MAX_GAME_PLAYED;i++){
        if (game[i].game_id==0) continue;
        if (game[i].game_id == chosen_game) {
            for (int j=0;j<game[i].no_of_player;j++){
                int pid = game[i].player_id[j];
                if (pid != player_id) {
                    for (int k=0;k<MAX_PLAYERS;k++){
                        if (player_arr[k].id==0) continue;
                        if (player_arr[k].id == pid) {
                            printf("Player Name: %s (ID: %d)\n", player_arr[k].name, player_arr[k].id);
                            found_any = 1;
                            break;
                        }
                    }
                }
            }
        }
    }
    if (!found_any) printf("No other players found for this game.\n");
}

void inactive_user(player player[]) {
    printf("-----------------INACTIVE PLAYERS-----------------\n");
    for (int i=0;i<MAX_PLAYERS;i++){
        if (player[i].id==0) continue;
        if (player[i].game_played.total_game_played==0) {
            printf("id: %d - %s\n", player[i].id, player[i].name);
        }
    }
}

void active_user(player player[]) {
    int k;
    printf("Enter the threshold game limit (1-4): ");
    if (scanf("%d", &k) != 1) return;
    for (int i=0;i<MAX_PLAYERS;i++){
        if (player[i].id==0) continue;
        if (player[i].game_played.total_game_played > k) {
            printf("Active user, id: %d - %s\n", player[i].id, player[i].name);
        }
    }
}

void most_active_game(player player[], play_game game[], game_master gm[]) {
    int game_play_count[MAX_GAME_PLAYED] = {0};
    for (int i=0;i<MAX_GAME_PLAYED;i++){
        if (game[i].game_id==0) continue;
        game_play_count[game[i].game_id]++;
    }
    int max_count=0, most_active_game_id=-1;
    for (int i=1;i<=100;i++){
        if (game_play_count[i] > max_count) { max_count = game_play_count[i]; most_active_game_id=i; }
    }
    if (most_active_game_id != -1) {



        printf("most active game is: %s (ID: %d) with %d plays\n", gm[most_active_game_id-1].name, most_active_game_id, max_count);
    } else {
        printf("No games have been played yet.\n");
    }
}

void top_5_games_by_total_days(play_game game[], game_master gm[]) {
    int total_days[201] = {0};
    for (int i=0;i<MAX_GAME_PLAYED;i++){
        if (game[i].game_id==0) continue;
        total_days[game[i].game_id] += game[i].days;
    }
    for (int c=0;c<5;c++){
        int max_days=0, max_id=0;
        for (int i=1;i<=200;i++){
            if (total_days[i] > max_days) { max_days = total_days[i]; max_id = i; }
        }
        if (max_id == 0) break;
        for (int g=0; g<MAX_GAMES; g++){
            if (gm[g].game_id==0) continue;
            if (gm[g].game_id == max_id) {
                printf("%d. Game ID: %d | Name: %s | Type: %s | Total Days: %d\n", c+1, max_id, gm[g].name, gm[g].type, max_days);
                break;
            }
        }
        total_days[max_id] = 0;
    }
}

void players_currentplaying(game_master gm[], play_game game_played[], player player[]) {
    int gid;
    for (int i=0;i<MAX_GAMES;i++) {
        if (gm[i].game_id==0) continue;
        printf("id: %d - %s\n", gm[i].game_id, gm[i].name);
    }
    printf("choose the game id whose current player you want to see: ");
    if (scanf("%d", &gid) != 1) return;
    int pid[50] = {0}, score[50] = {0}, count = 0;
    for (int i=0;i<MAX_GAME_PLAYED;i++){
        if (game_played[i].game_id==0) continue;
        if (game_played[i].game_id == gid) {
            for (int j=0;j<game_played[i].no_of_player;j++){
                pid[count] = game_played[i].player_id[j];
                score[count] = game_played[i].player_score[j];
                count++;
            }
        }
    }
    if (count == 0) { printf("no players are currently playing this game:\n"); return; }
    // sort by score desc
    for (int i=0;i<count-1;i++){
        for (int j=i+1;j<count;j++){
            if (score[i] < score[j]) {
                int tmp = score[i]; score[i] = score[j]; score[j] = tmp;
                int tid = pid[i]; pid[i] = pid[j]; pid[j] = tid;
            }
        }
    }
    printf("players currently playing game are:-\n");
    for (int i=0;i<count;i++){
        int arr_idx = pid[i] - 101;
        if (arr_idx >=0 && arr_idx < MAX_PLAYERS && player[arr_idx].id != 0) {
            printf("[player_name : %s , \t score : %d , \t prize : %d , \t id : %d ]\n",
                   player[arr_idx].name, score[i], player[arr_idx].game_played.prize, pid[i]);
        } else {
            printf("[player_id : %d , score: %d]\n", pid[i], score[i]);
        }
    }
}

void delete_guest(guest_users guest[]) {
    int changed = 0;
    for (int i=0;i<MAX_GUEST;i++){
        if (guest[i].id==0) continue;
        if (guest[i].days > 20) {
            printf("deleting guest user id: %d - %s\n", guest[i].id, guest[i].name);
            guest[i].id = 0; guest[i].days = 0; guest[i].name[0] = '\0';
            changed = 1;
        }
    }
    if (changed)
    save_guests(guest);
}

// ----------------------------- main --------------------------------
int main() {
   
    player player_arr[MAX_PLAYERS] = {
        {101,"Prince",{1,11,2025},{4,0,400},{1,2,3}},
        {102,"Sam",{15,11,2025},{1,1,100},{1,4,3}},
        {103,"Rohit",{12,11,2025},{4,2,300},{5,6,7}},
        {104,"Aditi",{18,11,2025},{3,1,200},{2,8,9}},
        {105,"Vikram",{20,11,2025},{2,0,50},{7,3,10}},
        {106,"Tina",{22,11,2025},{2,1,150},{4,1,19}},
        {107,"Aryan",{25,11,2025},{1,1,100},{6,17,2}},
        {108,"Nisha",{27,11,2025},{2,2,200},{5,16,14}},
        {109,"Ravi",{28,11,2025},{0,0,0},{2,1,3}},
        {110,"Sneha",{29,11,2025},{0,0,0},{10,14,19}},
        {111,"Karan",{30,11,2025},{0,0,0},{5,6,8}},
        {112,"Meera",{1,12,2025},{0,0,0},{2,16,9}},
        {113,"Dev",{2,12,2025},{0,0,0},{1,10,3}}
    };
    game_master game[MAX_GAMES] = {
        {1,"chess","multiuser",2},
        {2,"ludo","multiuser",4},
        {3,"carrom","multiuser",4},
        {4,"clashofclan","singleuser",1},
        {5,"cricketclash","singleuser",1},
        {6,"footballstrike","singleuser",1},
        {7,"carddeck","multiuser",4},
        {8,"tabletennis","multiuser",2},
        {9,"badmintonduel","multiuser",2},
        {10,"poolchamp","multiuser",2},
        {11,"racingfever","singleuser",1},
        {12,"sniperzone","singleuser",1},
        {13,"mysteryhunt","singleuser",1},
        {14,"battlearena","multiuser",4},
        {15,"minigolftour","multiuser",2},
        {16,"archeryking","singleuser",1},
        {17,"dragrace","multiuser",2},
        {18,"wordconnect","singleuser",1},
        {19,"quizmaster","multiuser",4},
        {20,"spaceinvader","singleuser",1}
    };
    play_game game_played[MAX_GAME_PLAYED] = {
        {1,2,{101,102},{90,95},102,{20,22},25},
        {1,2,{103,104},{85,80},103,{21,19},30},
        {1,2,{105,106},{78,90},106,{18,22},28},
        {4,1,{107},{100},107,{25},28},
        {4,1,{108},{120},108,{27},29},
        {6,2,{101,103},{88,92},103,{19,21},24},
        {7,2,{104,105},{100,94},104,{20,19},23},
        {10,2,{106,108},{89,95},108,{22,23},27},
        {11,2,{109,110},{75,80},110,{15,18},26},
        {12,1,{111},{85},111,{12},22},
        {13,1,{112},{90},112,{14},18},
        {14,4,{113,101,102,103},{70,80,75,85},103,{20,22,21,23},30}
    };
    guest_users guest[MAX_GUEST] = {
        {201,"guest1",5},
        {202,"guest2",3},
        {203,"guest3",10},
        {204,"guest4",28},
        {205,"guest5",24},
        {206,"guest6",6},
        {207,"guest7",19},
        {208,"guest8",8},
        {209,"guest9",24},
        {210,"guest10",25}
    };

    load_players(player_arr);
    load_games(game);
    load_game_played(game_played);
    load_guests(guest);

    int choice;
    do {
        printf("\n================= GAME DATABASE MENU =================\n");
        printf("1. Add New Player\n");
        printf("2. Update Player Game Details\n");
        printf("3. Show Player's Game Info\n");
        printf("4. Show Inactive Users (never played)\n");
        printf("5. Show Active Users (above threshold)\n");
        printf("6. Show Most Active Game (by play count)\n");
        printf("7. Show Top 5 Games by Total Days\n");
        printf("8. Show Players Currently Playing a Game (sorted by score)\n");
        printf("9. Delete Guest Users (Days > 20)\n");
        printf("10. Exit\n");
        printf("------------------------------------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); choice = 0; }
        switch (choice) {
            case 1: add(player_arr, game, game_played); break;
            case 2: update(player_arr, guest); break;
            case 3: game_info(player_arr, game_played, game); break;
            case 4: inactive_user(player_arr); break;
            case 5: active_user(player_arr); break;
            case 6: most_active_game(player_arr, game_played, game); break;
            case 7: top_5_games_by_total_days(game_played, game); break;
            case 8: players_currentplaying(game, game_played, player_arr); break;
            case 9: delete_guest(guest); break;
            case 10: printf("Exiting the system... Thank you!\n"); break;
            default: printf("Invalid choice! Please try again.\n"); break;
        }
    } while (choice != 10);
    return 0;


}
