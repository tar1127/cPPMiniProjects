/*
    Author: Tanvir Rasel

    Command line game of Blackjack

*/

#include <iostream>
#include <array>		// std::array
#include <random>       // std::random_device rd;
#include <algorithm>
#include <vector>

// card ranks 
enum class RANKS
{
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE,
    MAX_RANKS
};

// card suit 
enum class SUITS
{
    HEART,
    DIMOND,
    CLUB,
    SPADE,
    MAX_SUITS
};

// game results 
enum class GAME_RESULT {
    PLAYER_WIN,
    DEALER_WIN,
    TIE
};

// a card struct has rank and suit 
struct CARD
{
    RANKS rank;
    SUITS suit;
};

/*
    function prints the given card in two character format.
    number or face value and the suits.
    definations:
        RANKS:
          # = 2- 9
          T = Ten
          J=  Jack
          Q = Queen
          K = King
          A = Ace
        SUITS:
          S = Spade
          D = Dimonds
          C = Clubs
          H = Hearts

    i.e.  2H = 2 of hearts
          TS = 10 of Spades
          QD = Queen of Dimonds.

    PARAMS: struct CARD
    RETURN: NONE
*/
void printCard(const CARD card, bool hideCard){

    if (hideCard) {
        std::cout << (char)178; 
    }
    else {
        switch (card.rank)
        {
        case (RANKS::TWO):
            std::cout << "2";
            break;
        case (RANKS::THREE):
            std::cout << "3";
            break;
        case (RANKS::FOUR):
            std::cout << "4";
            break;
        case (RANKS::FIVE):
            std::cout << "5";
            break;
        case (RANKS::SIX):
            std::cout << "6";
            break;
        case (RANKS::SEVEN):
            std::cout << "7";
            break;
        case (RANKS::EIGHT):
            std::cout << "8";
            break;
        case (RANKS::NINE):
            std::cout << "9";
            break;
        case (RANKS::TEN):
            std::cout << "T";
            break;
        case (RANKS::JACK):
            std::cout << "J";
            break;
        case (RANKS::QUEEN):
            std::cout << "Q";
            break;
        case (RANKS::KING):
            std::cout << "K";
            break;
        case (RANKS::ACE):
            std::cout << "A";
            break;
        default:
            std::cout << "?";
            break;
        }

        switch (card.suit)
        {
        case (SUITS::DIMOND):
            std::cout << "D";
            break;
        case (SUITS::HEART):
            std::cout << "H";
            break;
        case (SUITS::CLUB):
            std::cout << "C";
            break;
        case (SUITS::SPADE):
            std::cout << "S";
            break;
        default:
            std::cout << "?";
            break;
        }

    }


}


/*
    A deck a array of 52 cards with each element is a  CARD struct.
*/
using deck_type = std::array<CARD, 52>;
deck_type createDeck()
{
    deck_type deck;
    int index = 0;

    for (int i = 0; i != static_cast<int>(SUITS::MAX_SUITS); ++i) {
        for (int j = 0; j != static_cast<int>(RANKS::MAX_RANKS); ++j) {
            deck[index].rank = static_cast<RANKS>(j);
            deck[index].suit = static_cast<SUITS>(i);
            index++;
        }
    }
    return deck;
}

// print the entire deck 
void printDeck(const deck_type& deck) {
    for (const auto& card : deck) {
        printCard(card, false);
        std::cout << " ";
    }
    std::cout << '\n';
}

// function to shuffel the deck 
void shuffleDeck(deck_type& deck) {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(deck.begin(), deck.end(), g);
}


// get card numeric value 
int getCardValue(const CARD& card, int aceCount = 0, int currSum = 0) {
    int tempSum = currSum;
    if (card.rank < RANKS::TEN) {
        return static_cast<int>(card.rank) + 2;
    }
    else {
        switch (card.rank) {
        case(RANKS::TEN):
        case(RANKS::JACK):
        case(RANKS::QUEEN):
        case(RANKS::KING):
            return 10;
        case(RANKS::ACE):
            // special case for Ace. 
            for (int j = 0; j < aceCount; j++) {

                if (currSum + 11 > 21) {
                    tempSum += 1;
                }
                else {
                    tempSum += 11;
                }
            }
            currSum = tempSum; 

            return currSum;

        default:
            return 0;
        }

    }

}


// calculate card sum 
int cardSumCal(const std::vector<CARD>& arr) {
    int sum = 0;
    int aceCount = 0;
    CARD aceCard;
    for (const auto& c : arr) {
        if (c.rank == RANKS::ACE) {
            aceCount++;
            aceCard = c;
            continue;
        }
        else {
            sum += getCardValue(c);
        }

    }

    if (aceCount > 0) {
        sum = getCardValue(aceCard, aceCount, sum);
    }


    return sum;

}

/*
    Blackjack game play function.

*/

// draw header 
void drawTerminalOut() {
    std::cout << "\t\t\t\t" << "************************************************************\n";
    std::cout << "\t\t\t\t" << "**                                                        **\n";
    std::cout << "\t\t\t\t" << "**                   ##################                   **\n";
    std::cout << "\t\t\t\t" << "**                   # BLACKJACK GAME #                   **\n";
    std::cout << "\t\t\t\t" << "**                   ##################                   **\n";
    std::cout << "\t\t\t\t" << "**                                                        **\n";
    std::cout << "\t\t\t\t" << "************************************************************\n";
    std::cout << "\t\t\t\t" << "   PLAYER HAND                              DEALER HAND     \n";
    std::cout << "\t\t\t\t" << "  ==============                           ==============   \n";
}



GAME_RESULT playBlackJack(const deck_type& sDeck) {
    bool gameOver = false;
    int topCard = 0;

    GAME_RESULT result;
    std::vector<CARD> playerHand(2);
    std::vector<CARD> dealerHand(2);

    // intital deal with random card 
    for (int i = 0; i < 2; i++) {
        playerHand[i] = sDeck[topCard];
        topCard++;
        dealerHand[i] = sDeck[topCard];
        topCard++;
    }


    while (!gameOver) {
        int dealerSum = 0;
        int playerSum = 0;
        std::cout << "\t\t\t ";
        printCard(playerHand[0], false);
        std::cout << " ";
        printCard(playerHand[1], false);
        std::cout << "\t\t\t\t\t";
        printCard(dealerHand[0], false);
        std::cout << " ";
        printCard(dealerHand[1],true);
        // calculate card sum. 
        playerSum = cardSumCal(playerHand);
        dealerSum = cardSumCal(dealerHand);

        std::cout << "\n";
        std::cout << "PLAYER TOTAL: " << playerSum << "\n";
        std::cout << "DEALER TOTAL: " << dealerSum << "\n";
        if (playerSum > dealerSum) {

            result = GAME_RESULT::PLAYER_WIN;
        }
        else if (playerSum < dealerSum) {
            result = GAME_RESULT::DEALER_WIN;
        }
        else {
            result = GAME_RESULT::TIE;
        }
        gameOver = true;
    }

    return result;

}


int main() {

    drawTerminalOut();
    auto deck{ createDeck() };
    printDeck(deck);
   
    shuffleDeck(deck);
    std::cout << "\n";
    printDeck(deck);
    std::cout << "\n";
    //std::cout << getCardValue(deck[0]);
    // TEST: print the cards in the deck 
    // for (auto c: deck){
    //     printCard(c);
    //     std::cout << " "; 
    // }

    //std::cout << (char)179; 
    switch (playBlackJack(deck)) {

    case(GAME_RESULT::PLAYER_WIN):
        std::cout << "YOU WIN!!!";
        break;
    case(GAME_RESULT::DEALER_WIN):
        std::cout << "DEALER WIN";
        break;
    case(GAME_RESULT::TIE):
        std::cout << "TIE";
        break;
    default:
        // should not happen
        std::cout << "somethings wrong";
        break;

    }


}