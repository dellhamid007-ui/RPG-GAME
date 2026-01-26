#ifndef ITEMS_H
#define ITEMS_H


typedef struct Player Player;

typedef enum itemType{
    Sword,
    Potion,
    Shield
}itemType;

typedef enum Rarity{
    Common,
    Uncommon,
    Rare,
    Mythical,
    Legendary
}Rarity;

typedef struct Item{
    itemType type;
    Rarity rarity;
    int value; //damage if sword, HP if potion, and 1 if shield
}Item;

Item* createItem(itemType type, Rarity rarity);
int calcValue(Item item); //depending on rarity
void destroyItem(Item** item);



#endif 