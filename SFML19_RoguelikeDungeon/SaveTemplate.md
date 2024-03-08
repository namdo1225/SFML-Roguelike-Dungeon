# Save Format
Save file MUST have the extension ```.sav```.

```json
{
	name: string,
	floorNumber: int,
	level: int,
	gold: int,
	statPoints: int,
	player: {
		x: int,
		y: int
	},
	exp: {
		current: int,
		levelUp: int
	},
	inventorySlots: int,
	stat: {
		hp: {
			current: int,
			max: int
		},
		mp: {
			current: int,
			max: int
		},
		str: int,
		mgk: int,
		def: int,
		res: int
	},
	inventory: [int],
	weaponSlot: int,
	armorSlot: int,
	spell: [int],
	floor: [
		{
			x: int,
			y: int,
			sizeX: int,
			sizeY: int,
			door: {
					x: int,
					y: int,
					rotation: int,
					door0: 0 | 1,
					door1: 0 | 1,
					door2: 0 | 1,
					door3: 0 | 1
			}
		}
	],
	stair: {
		x: int,
		y: int
	},
	shop: {
		x: int,
		y: int
	},
	enemiesRespawn: int,
	enemies: [
		{
			id: int,
			health: int,
			x: int,
			y: int
		}
	],
	items: [
		{
			id: int,
			x: int,
			y: int
		}
	],
	golds: [
		{
			amount: int,
			x: int,
			y: int
		}
	],
	interactibles: [
		{
			x: int,
			y: int
		}
	],
	effects: [
		{
			stat: int,
			difference: int,
			turn: int,
			original: int
		}
	]
}
```