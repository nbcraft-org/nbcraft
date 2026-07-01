#pragma once

#include "Item.hpp"

class FoodItem : public Item
{
public:
	FoodItem(int id, int nutrition, bool wolfFood = false);
	
	bool use(ItemStack&, Level*, Mob&) const override;

	bool isFood() const override { return true; };
	bool isWolfFood() const override { return m_bIsWolfFood; }
	int getNutrition() const { return m_nutrition; }

protected:
	int m_nutrition;
	bool m_bIsWolfFood;
};