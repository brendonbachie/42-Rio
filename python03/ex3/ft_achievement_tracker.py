import random

def gen_player_achievements() -> set:
    achievements = {'Crafting Genius', 'Strategist', 'World Savior', 'Speed Runner', 'Survivor',
'Master Explorer', 'Treasure Hunter', 'Unstoppable', 'First Steps', 'Collector Supreme', 'Untouchable', 
'Sharp Mind', 'Boss Slayer'}
    return set(random.sample(achievements, random.randint(0, len(achievements))))

def main() -> None:
     print("=== Achievement Tracker System ===")
     players = ['Alice', 'Bob', 'Charlie', 'Dylan']
     for player in players:
          player = gen_player_achievements()





Player Alice: {}
Player Bob: {'Crafting Genius', 'Strategist', 'World Savior', 'Master Explorer', 'Unstoppable', '
Collector Supreme', 'Untouchable'}
Player Charlie: {'Strategist', 'Speed Runner', 'Survivor', 'Master Explorer', 'Treasure Hunter', 'First
Steps', 'Collector Supreme', 'Untouchable', 'Sharp Mind'}
Player Dylan: {'Strategist', 'Speed Runner', 'Unstoppable', 'Untouchable', 'Boss Slayer'}
All distinct achievements: {'Crafting Genius', 'Strategist', 'World Savior', 'Speed Runner', 'Survivor',
'Master Explorer', 'Treasure Hunter', 'Unstoppable', 'First Steps', 'Collector Supreme', '
Untouchable', 'Sharp Mind', 'Boss Slayer'}
Common achievements: {'Untouchable'}
Only Alice has: set()
Only Bob has: set()
Only Charlie has: {'Survivor', 'Treasure Hunter', 'First Steps', 'Sharp Mind'}
Only Dylan has: set()
Alice is missing: {'Strategist', 'Speed Runner', 'Survivor', 'Treasure Hunter', 'Unstoppable', 'Hidden
Path Finder', 'First Steps', 'Sharp Mind'}
Bob is missing: {'Speed Runner', 'Survivor', 'Treasure Hunter', 'Hidden Path Finder', 'First Steps', '
Sharp Mind', 'Boss Slayer'}
Charlie is missing: {'Crafting Genius', 'World Savior', 'Hidden Path Finder', 'Unstoppable', 'Boss
Slayer'}
Dylan is missing: {'Crafting Genius', 'World Savior', 'Survivor', 'Master Explorer', 'Treasure Hunter',
'Hidden Path Finder', 'First Steps', 'Collector Supreme', 'Sharp Mind'}