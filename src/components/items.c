#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"
#include "enemy.h"


Item* createItem(itemType type, Rarity rarity){
    Item* item = malloc(sizeof(Item));
    item->rarity = rarity;
    item->type = type;
    item->value = calcValue(*item);

    return item;
}

int calcValue(Item item){
    switch(item.type){
        case Sword: switch(item.rarity){
            case Common: return 5; break;
            case Uncommon: return 10; break;
            case Rare: return 15; break;
            case Mythical: return 20; break;
            case Legendary: return 25; break;
        } break;
        case Potion: switch(item.rarity){
            case Common: return 10; break;
            case Uncommon: return 20; break;
            case Rare: return 30; break;
            case Mythical: return 40; break;
            case Legendary: return 50; break;
        }break;
        case Shield: return 1;
    }
}

void destroyItem(Item** item){
    if (item == NULL || *item == NULL) return;
    
    free(*item);
    *item = NULL;
}