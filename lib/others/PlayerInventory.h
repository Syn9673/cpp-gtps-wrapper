#ifndef PLAYER_INVENTORY_H
#define PLAYER_INVENTORY_H

struct PlayerInventory {
	std::vector<InventoryItem> items;
	int inventorySize = 100;
};

#endif