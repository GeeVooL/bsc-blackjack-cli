//
//  main.c
//  Gra w oczko 0.3
//
//  Created by Michał Dziewulski on 19.11.2016.
//  Copyright © 2016 Michał Dziewulski. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


//Multiplatform macro
#if __APPLE__ || __unix__ || __unix || unix
# include <unistd.h>     //UNIX standard lib
# define CLRCMD "clear"  //UNIX terminal clear command
#elif _WIN32
# include <windows.h>    //Windows lib
# define CLRCMD "cls"    //Windows console clear command
#endif
//~Multiplatform macro

//Preprocessor definitions
#define SSTRLEN 10       //Short string length
#define LSTRLEN 30       //Long string length
#define SETSIZE 15       //Player's set size
#define SUITN 4          //Number of suits
#define FIGN 13          //Number of figures
//~Preprocessor definition

//CARD TYPEDEF
typedef
    struct
    {
        int value;
        int amount;
        char name[LSTRLEN];
    }
card;

enum suits
{
    CLUB, //Trefl
    DIAMOND, //Karo
    HEART, //Kier
    SPADE //Pik
};
//~CARD TYPEDEF

//FUNCTION PROTOTYPES
void init_cardset(card[SUITN][FIGN]); //Initialize cardset
void show_header(void); //Show header
int menu(void); //Show menu and return chosen option
void game(void); //Whole game logic and interface
int comp_decision(int, card[SUITN][FIGN]); //Computer decision based on probability
void take_card(card[SUITN][FIGN], card[SETSIZE], int); //Take random card from cardset and add it player's set
void game_over(card[SETSIZE], card[SETSIZE], int, int, int, int); //Game over logic and interface
void mp_sleep(unsigned); //Universal multiplatform sleep function that takes SECONDS as argument
//~FUNCTION PROTOTYPES


//MAIN
int main(void) {
    
    system(CLRCMD); //Clear console
    srand((unsigned)time(NULL));
    
    while(1)
    {
        system(CLRCMD);
        int option = menu(); //Display menu
        
        switch (option)
        {
            case 1:
                game();
                break;
            case 2:
                system(CLRCMD);
                puts("Program zakonczony.");
                return 0;
        }
    }
}
//~MAIN

//FUNCTION DEFINITIONS

//Universal sleep function
void mp_sleep(unsigned sec)
{
    
#if __APPLE__ || __unix__ || __unix || unix
    sleep(sec);
#elif _WIN32
    Sleep(sec * 1000);
#endif
    
}
//~Universal sleep function

//Show header
void show_header()
{
    printf("---------------------------------------------------\n");
    printf("Gra w oczko\n");
    printf("Autor: Michal Dziewulski (1I2; sroda, godz. 10)\n");
    printf("---------------------------------------------------\n\n");
}
//~Show header

//Menu
int menu()
{
    int option;
    
    show_header();
    printf("MENU:\n");
    printf("1. Rozpocznij gre z komputerem\n");
    printf("2. Zakoncz program\n\n");
    printf("Wprowadz wybrana opcje: ");
    
    while(scanf("%d", &option) != 1 || (option != 1 && option != 2)) //Input control
    {
        puts("Bledne dane. Sprobuj ponownie.");
        mp_sleep(1);
        system(CLRCMD);
        
        show_header();
        printf("MENU:\n");
        printf("1. Rozpocznij gre z komputerem\n");
        printf("2. Zakoncz program\n\n");
        printf("Wprowadz wybrana opcje: ");
        while(getchar() != '\n');
    }
    
    return option;
}
//~Menu

//Game
void game()
{
    card cardset[SUITN][FIGN]; //Create cardset
    init_cardset(cardset); //Initialize cardset
    
    card computer_set[SETSIZE]; //Computer's cards
    card player_set[SETSIZE]; //Player's cards
    
    int comp_dec = 0; //Computer's decision
    int pl_dec = 0; //Player's decision
    int comp_value = 0; //Current computer value
    int pl_value = 0; //Current player value
    int comp_pass = 0; //If computer passed
    int pl_pass = 0; //If player passed
    int cur_comp_card = 0; //Current computer's card in set
    int cur_pl_card = 0; //Current player's card in set
    
    //Initial deal
    for(int i = 0; i < 2; i++) //Two cards for everyone
    {
        take_card(cardset, player_set, cur_pl_card); //Player takes card
        pl_value += player_set[cur_pl_card].value;
        cur_pl_card++;
    
        take_card(cardset, computer_set, cur_comp_card); //Computer takes card
        comp_value += computer_set[cur_comp_card].value;
        cur_comp_card++;
    }
    
    //Two aces - win
    if(pl_value == 22 || comp_value == 22)
    {
        system(CLRCMD);
        printf("---------------------------------------------------\n");
        printf("KTOŚ MA PERSKIE OCZKO!\n");
        printf("---------------------------------------------------\n");
        mp_sleep(2);
        game_over(player_set, computer_set, cur_pl_card, cur_comp_card, pl_value, comp_value);
        return;
    }
    
    //Player's turn
    while(!pl_pass)
    {
        system(CLRCMD);
        
        //Interface - player's cards
        printf("Twoje karty:\n");
        
        for(int i = 0; i < cur_pl_card; i++)
        {
            printf("%s\n", player_set[i].name);
        }
        
        printf("\nSuma: %d\n", pl_value);
        printf("---------------------------------------------------\n"); //51 x '-'
        
        //Interface - player's decision
        if (pl_value > 21)
        {
            pl_dec = 2;
            printf("Przekroczyles 21 - wycofujesz sie\n");
            printf("---------------------------------------------------\n");
        }
        else
        {
            printf("Opcje: 1. Wez karte  2. Wycofaj sie  3. Zakoncz gre\n");
            printf("Wybierz opcje: ");
            while(scanf("%d", &pl_dec) != 1 || (pl_dec != 1 && pl_dec != 2 && pl_dec != 3)) //Input control
            {
                printf("Bledne dane. Sprobuj ponownie.\n");
                printf("Wybierz opcje: ");
                while(getchar() != '\n');
            }
            printf("---------------------------------------------------\n");
        }
        
        //Logic - player's decision
        if(pl_dec == 1)
        {
            puts("Dobrales karte.");
            take_card(cardset, player_set, cur_pl_card); //Player takes card
            pl_value += player_set[cur_pl_card].value; //Increase current value
            cur_pl_card++;
        }
        else if(pl_dec == 2) //Player passed
        {
            pl_pass = 1;
            puts("Wycofales sie.");
        }
        else //Exit game
        {
            system(CLRCMD);
            puts("Zakonczono gre.");
            return;
        }
        
        mp_sleep(2);
    }
    
    //Interface - header
    system(CLRCMD);
    printf("---------------------------------------------------\n");
    printf("KOMPUTER\n");
    printf("---------------------------------------------------\n");
    
    while (!comp_pass)
    {
        //Logic - current computer's decision
        
        //Computer passes if it's value is higher or equal 21, else it decides to hit or pass
        if(comp_value < 21)
            comp_dec = comp_decision(comp_value, cardset); //1 - hit, 0 - pass
        else
            comp_dec = 0; //0 - pass
        
        if(comp_dec) //Hit
        {
            puts("Komputer dobral karte.");
            take_card(cardset, computer_set, cur_comp_card); //Computer takes card
            comp_value += computer_set[cur_comp_card].value; //Increase current value
            cur_comp_card++;
        }
        else
        {
            comp_pass = 1; //Computer passed
        }
        
        //Interface - computer's decision
        if(comp_pass)
        {
            puts("Komputer wycofal sie.");
        }
        
        //Interface - sleep before clearing screen in next iteration
        mp_sleep(2);
    
    }
    
    //Game over
    game_over(player_set, computer_set, cur_pl_card, cur_comp_card, pl_value, comp_value);
    return;
}
//~Game

//Game over
void game_over(card player_set[SETSIZE], card computer_set[SETSIZE], int cur_pl_card, int cur_comp_card, int pl_value, int comp_value)
{
    //Flags described below
    int pl_end_flag; //Player flag
    int comp_end_flag; //Computer flag
    
    //Interface - header
    system(CLRCMD);
    printf("---------------------------------------------------\n");
    printf("KONIEC GRY\n");
    printf("---------------------------------------------------\n");
    
    //Interface - players cards
    printf("Karty gracza:\n");
    
    for(int i = 0; i < cur_pl_card; i++)
    {
        printf("%s\n", player_set[i].name);
    }
    
    printf("\nSuma: %d\n", pl_value);
    printf("---------------------------------------------------\n");
    printf("Karty komputera:\n");
    
    for(int i = 0; i < cur_comp_card; i++)
    {
        printf("%s\n", computer_set[i].name);
    }
    
    printf("\nSuma: %d\n", comp_value);
    
    //Logic - who won
    
    //Flag are used to compare situations between players.
    //The player with lower flag is the winner.
    //Flags of the same value means draw (except 2 - it is required to compare values)
    //1 - value == 21, 2 - value < 21, 3 - value > 21
    
    //Two aces
    if(cur_comp_card - 1 == 1 && cur_pl_card - 1 == 1)
    {
        if(pl_value == 22 && comp_value == 22)
        {
            printf("---------------------------------------------------\n");
            printf("DWA PERSKIE OCZKA! REMIS!\n");
            printf("---------------------------------------------------\n");
        }
        else if(pl_value == 22)
        {
            printf("---------------------------------------------------\n");
            printf("PERSKIE OCZKO! WYGRYWA GRACZ!\n");
            printf("---------------------------------------------------\n");
        }
        else
        {
            printf("---------------------------------------------------\n");
            printf("PERSKIE OCZKO! WYGRYWA KOMPUTER!\n");
            printf("---------------------------------------------------\n");
        }
        
        printf("\nNacisnij ENTER, aby zakonczyc gre...\n");
        getchar();
        getchar();
        return;
    }
    
    //Player flag
    if (pl_value < 21)
        pl_end_flag = 2;
    else if (pl_value == 21)
        pl_end_flag = 1;
    else if (pl_value > 21)
        pl_end_flag = 3;
    
    //Computer flag
    if (comp_value < 21)
        comp_end_flag = 2;
    else if (comp_value == 21)
        comp_end_flag = 1;
    else if (comp_value > 21)
        comp_end_flag = 3;
    
    //Interface - who won
    if(pl_end_flag < comp_end_flag)
    {
        printf("---------------------------------------------------\n");
        printf("WYGRYWA GRACZ!\n");
        printf("---------------------------------------------------\n");
    }
    else if (pl_end_flag > comp_end_flag)
    {
        printf("---------------------------------------------------\n");
        printf("WYGRYWA KOMPUTER!\n");
        printf("---------------------------------------------------\n");
    }
    else
    {
        if(pl_end_flag == 2)
        {
            if(pl_value > comp_value)
            {
                printf("---------------------------------------------------\n");
                printf("WYGRYWA GRACZ!\n");
                printf("---------------------------------------------------\n");
                
            }
            else if (pl_value < comp_value)
            {
                printf("---------------------------------------------------\n");
                printf("WYGRYWA KOMPUTER!\n");
                printf("---------------------------------------------------\n");
            }
            else
            {
                printf("---------------------------------------------------\n");
                printf("REMIS!\n");
                printf("---------------------------------------------------\n");
            }
        }
        else
        {
            printf("---------------------------------------------------\n");
            printf("REMIS!\n");
            printf("---------------------------------------------------\n");
        }
    }
    
    //Interface - exit game
    printf("\nNacisnij ENTER, aby zakonczyc gre...\n");
    getchar();
    getchar();
    return;
}
//~Game over

//Computer's decision
int comp_decision(int value, card cardset[SUITN][FIGN])
{
    int target = 21 - value; //Maximum card's value that isn't higher than 21
    int low = 0, high = 0; //low - card that is equal or lower than target, high - higher than target
    
    //Loop checks every available card in cardset
    for(int i = 0; i < SUITN; i++)
    {
        for(int j = 0; j < FIGN; j++)
        {
            if(cardset[i][j].amount == 1)
            {
                if(cardset[i][j].value <= target)
                    low++;
                else
                    high++;
            }
        }
    }
    
    double probability = (double) low / (double)(low + high); //calculate probability of choosing low card from all available cards
    
    //1 - hit, 0 - pass
    return probability > 0.5;
}
//~Computer's decision

//Take a random card
void take_card(card cardset[SUITN][FIGN], card set[SETSIZE], int i)
{
    int card_suit;
    int card_fig;
    
    do
    {
        card_suit = rand() % SUITN;
        card_fig = rand() % FIGN;
    } while(cardset[card_suit][card_fig].amount != 1);
    
    set[i] = cardset[card_suit][card_fig]; //add card to player's set
    cardset[card_suit][card_fig].amount = 0;
}
//~Take a random card

//Initialization of a cardset (hardcoded because it's easier, faster and the code is more readable this way)
void init_cardset(card cardset[SUITN][FIGN])
{
    for(int i = 0; i < SUITN; i++)
    {
        char suit_name[SSTRLEN] = "";
        switch(i)
        {
            case CLUB:
                strncpy(suit_name, "TREFL", SSTRLEN);
                break;
            case DIAMOND:
                strncpy(suit_name, "KARO", SSTRLEN);
                break;
            case HEART:
                strncpy(suit_name, "KIER", SSTRLEN);
                break;
            case SPADE:
                strncpy(suit_name, "PIK", SSTRLEN);
                break;
        }
        
        //ACE
        cardset[i][0].amount = 1;
        cardset[i][0].value = 11;
        strncpy(cardset[i][0].name, suit_name, LSTRLEN);
        strcat(cardset[i][0].name, " AS");
        
        //2
        cardset[i][1].amount = 1;
        cardset[i][1].value = 2;
        strncpy(cardset[i][1].name, suit_name, LSTRLEN);
        strcat(cardset[i][1].name, " 2");
        
        //3
        cardset[i][2].amount = 1;
        cardset[i][2].value = 3;
        strncpy(cardset[i][2].name, suit_name, LSTRLEN);
        strcat(cardset[i][2].name, " 3");
        
        //4
        cardset[i][3].amount = 1;
        cardset[i][3].value = 4;
        strncpy(cardset[i][3].name, suit_name, LSTRLEN);
        strcat(cardset[i][3].name, " 4");
        
        //5
        cardset[i][4].amount = 1;
        cardset[i][4].value = 5;
        strncpy(cardset[i][4].name, suit_name, LSTRLEN);
        strcat(cardset[i][4].name, " 5");
        
        //6
        cardset[i][5].amount = 1;
        cardset[i][5].value = 6;
        strncpy(cardset[i][5].name, suit_name, LSTRLEN);
        strcat(cardset[i][5].name, " 6");
        
        //7
        cardset[i][6].amount = 1;
        cardset[i][6].value = 7;
        strncpy(cardset[i][6].name, suit_name, LSTRLEN);
        strcat(cardset[i][6].name, " 7");
        
        //8
        cardset[i][7].amount = 1;
        cardset[i][7].value = 8;
        strncpy(cardset[i][7].name, suit_name, LSTRLEN);
        strcat(cardset[i][7].name, " 8");
        
        //9
        cardset[i][8].amount = 1;
        cardset[i][8].value = 9;
        strncpy(cardset[i][8].name, suit_name, LSTRLEN);
        strcat(cardset[i][8].name, " 9");
        
        //10
        cardset[i][9].amount = 1;
        cardset[i][9].value = 10;
        strncpy(cardset[i][9].name, suit_name, LSTRLEN);
        strcat(cardset[i][9].name, " 10");
        
        //JACK
        cardset[i][10].amount = 1;
        cardset[i][10].value = 2;
        strncpy(cardset[i][10].name, suit_name, LSTRLEN);
        strcat(cardset[i][10].name, " WALET");
        
        //QUEEN
        cardset[i][11].amount = 1;
        cardset[i][11].value = 3;
        strncpy(cardset[i][11].name, suit_name, LSTRLEN);
        strcat(cardset[i][11].name, " KROLOWA");
        
        //KING
        cardset[i][12].amount = 1;
        cardset[i][12].value = 4;
        strncpy(cardset[i][12].name, suit_name, LSTRLEN);
        strcat(cardset[i][12].name, " KROL");
        
    }
}
//~Initialization of a cardset

//~FUNCTIONS DEFINITIONS
