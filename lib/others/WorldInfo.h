#ifndef WORLDINFO_H
#define WORLDINFO_H

struct WorldInfo {
	int width = 100;
	int height = 60;
	std::string name = "NODEJS";
	WorldItem* items;
	std::string owner = "";
	bool isPublic=false;

	unsigned long currentItemUID = 1; //has to be 1 by default
	std::vector<DroppedItem> droppedItems;
};

#endif