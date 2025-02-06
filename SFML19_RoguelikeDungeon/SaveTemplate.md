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
	weaponID: int,
	armorID: int,
	spell: [int],
	floor: [
		{
			x: int,
			y: int,
			sizeX: int,
			sizeY: int,
			doors: [{
					x: int,
					y: int,
					size: int,
					direction: int,
					fromRoomId: int,
					toRoomId: int
			}]
			visited: boolean
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
			y: int,
			hidden: boolean
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