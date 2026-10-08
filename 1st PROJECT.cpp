# include <iostream>
# include <string>
# include <vector>

using namespace std;

class Character 
{
	protected:
		string name;
		int health;
		int attackPower;
	
	public:
    	Character(string name,int health,int attackPower)
    	{
    		this->name=name;
    		this->health=health;
    		this->attackPower=attackPower;
		}
		
		virtual ~Character()
        {
        	
        }
		
		void displayStats()
		{
			cout<<"Name: "<<name<<"\n";
			cout<<"Health: "<<health<<"\n";
			cout<<"Attack Power: "<<attackPower<<"\n";
		}
		
		void takeDamage(int amount)
		{
			health -=amount;
		}
		
		int getAttackPower()
        {
            return attackPower;
        }
		
        string getName()
        {
        	return name;
		}
		
        virtual void attack(Character& target)
        {
            cout << name << " attacks " << target.getName() << "!\n";
            target.takeDamage(attackPower);
        }
        
        void heal(int amount)
        {
        	health +=amount;
		}
		
		int getHealth()
		{
			return health;
		}
		
};

class Weapon
{
    private:
        string weaponName;
        int damage;
    public:
        Weapon(string w, int d)
        {
        	weaponName=w;
        	damage=d;
		}
        int getDamage()
        {
        	return damage;
		}
        string getWeaponName()
        {
        	return weaponName;
		}
};
class Player;
class Item 
{	
    protected:
        string itemName;
    public:
        Item(string itemName)
        {
            this->itemName = itemName;
        }
        
        virtual ~Item()
        {
        	
        }
        string getName()
        {
            return itemName;
        }
        virtual void use(Player& target) = 0;
};

class Player : public Character 
{
	private:
		int Level;
		Weapon* weapon;
		vector<Item*> inventory;
		int xp;
        int xpToNextLevel;
	public:
	    Player(string name, int health, int attackPower) : Character(name, health, attackPower)
        {
        	xp=0;
        	xpToNextLevel=100;
            Level = 1;
            weapon=nullptr;
        }
        
        ~Player()
        {
             delete weapon;

            for (Item* item : inventory)
            {
                delete item;
            }

            inventory.clear();
        }
       
        void displayStats()
        {
            Character::displayStats();
            cout << "Level: " << Level << "\n";
        }
        
        void equipWeapon(Weapon* w)
        {
            weapon = w;
        }
        
        void attack(Character& target)
        {
        	if (weapon != nullptr)
        	{
        		cout << name << " attacks " << target.getName() << " with " << weapon->getWeaponName() << "!\n";
        		target.takeDamage(attackPower+weapon->getDamage());
			} else
			{
            cout << name << " attacks " << target.getName() << "!\n";
            target.takeDamage(attackPower);
            }
        }

        void addItem(Item* item)
        {
        	inventory.push_back(item);
		}
		
		void showInventory()
		{
			if (! inventory.empty())
			{
				cout<<"==== INVENTORY ====\n";
			int i=1;	
			for(auto inv : inventory)
			{
				cout<<i<<"."<<inv->getName()<<"\n";
				i++;
			}
			} else
			{
			    cout<<"Inventory is empty.\n";
		    }
		}
		
		Item* getItem(int index)
        {
            if (index >= 0 && index < inventory.size())
            {
                return inventory[index];
            }
                return nullptr;
        }

        int getInventorySize()
        {
            return inventory.size();
        }
     
		void gainXP(int amount)
        {
            xp += amount;
            cout << name << " gains " << amount << " XP!\n";

            if (xp >= xpToNextLevel)
            {
                Level += 1;
                health += 20;
                attackPower += 5;
                xp = 0;
                xpToNextLevel = xpToNextLevel * 1.5;
                cout << "Level Up! " << name << " is now Level " << Level << "!\n";
                cout << "New Health: " << health << ", New Attack Power: " << attackPower << "\n";
            }
        }
};

class Enemy : public Character
{
	protected:
		int xpReward;
    public:
    	Enemy(string name,int health,int attackPower,int xp) : Character (name,health,attackPower)
    	{
    		xpReward=xp;
		}
		
		int getXPReward()
        {
                return xpReward;
        }
};

class Goblin : public Enemy
{
	public:
		Goblin(string name,int health,int attackPower,int xp) : Enemy (name,health,attackPower,xp)
    	{
    		
		}
		  
	    void attack(Character& target)
        {
            cout<<"Goblin scratches at you!\n";
            target.takeDamage(attackPower);
        }
		
};

class Orc : public Enemy
{
	public:
		Orc(string name,int health,int attackPower,int xp) : Enemy (name,health,attackPower,xp)
    	{
    		
		}
		
		void attack(Character& target)
        {
            cout<<"Orc smashes you with a club!\n";
            target.takeDamage(attackPower * 2);
        }
};

class HealthPotion : public Item
{
	protected:
		int healAmount;
	public:
		HealthPotion(int ha,string itN):Item (itN)
		{
			healAmount=ha;
		}
		int getHealAmount()
		{
			return healAmount;
		}
		void use(Player& target)
        {
            target.heal(healAmount);
            cout << target.getName() << " uses " << itemName << " and heals " << healAmount << " HP!\n";
        }
		
};

void battle(Player& player, Enemy& enemy)
{
    while (player.getHealth() > 0 && enemy.getHealth() > 0)
    {
        cout << "===== BATTLE =====\n";
        cout << "Player: " << player.getName() << "\nHP: " << player.getHealth() << "\n\n";
        cout << "Enemy: " << enemy.getName() << "\nHP: " << enemy.getHealth() << "\n\n";
        cout << "1. Attack\n2. Use Item\n3. Run\nChoose: ";

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            player.attack(enemy);
            if (enemy.getHealth() > 0)
                enemy.attack(player);
        }
           else if (choice == 2)
        {
             player.showInventory();
             if (player.getInventorySize() > 0)
           {
                 cout << "Choose item number: ";
                 int itemChoice;
                 cin >> itemChoice;
                 Item* selected = player.getItem(itemChoice - 1);
                 if (selected != nullptr)
           {
            selected->use(player);
            enemy.attack(player); 
           }
           
           }
        }
        else if (choice == 3)
        {
            cout << "You fled the battle.\n";
            break;
        }

        if (enemy.getHealth() <= 0)
        {
            cout << "You defeated the " << enemy.getName()<< "!\n";
            player.gainXP(enemy.getXPReward());
        }
        else if (player.getHealth() <= 0)
        {
            cout << "You were defeated...\n";
        }
    }
}

int main()
{	
    Player p1("Alex", 100, 20);

    HealthPotion* potion = new HealthPotion(30, "Health Potion");
    p1.addItem(potion);
    
    Weapon* sword = new Weapon("Iron Sword", 15);
    p1.equipWeapon(sword);

    bool gameOver = false;

    while (!gameOver)
    {
        cout << "\n===== WORLD =====\n";
        cout << "1. Village (safe, no enemies)\n";
        cout << "2. Forest (Goblins)\n";
        cout << "3. Cave (Orcs)\n";
        cout << "4. Check Stats\n";
        cout << "5. Quit\n";
        cout << "Choose: ";

        int choice;
        cin >> choice;

        if (choice == 1)
        {
            cout << "You rest safely in the village.\n";
        }
        else if (choice == 2)
        {
            Goblin goblin("Goblin", 100, 10, 120);
            battle(p1, goblin);

            if (p1.getHealth() <= 0)
            {
                cout << "\nGAME OVER. " << p1.getName() << " has fallen.\n";
                gameOver = true;
            }
        }
        else if (choice == 3)
        {
            Orc orc("Orc", 80, 15, 150);
            battle(p1, orc);

            if (p1.getHealth() <= 0)
            {
                cout << "\nGAME OVER. " << p1.getName() << " has fallen.\n";
                gameOver = true;
            }
        }
        else if (choice == 4)
        {
            p1.displayStats();
        }
        else if (choice == 5)
        {
            cout << "Thanks for playing. Goodbye!\n";
            gameOver = true;
        }
    }

    return 0;
}
