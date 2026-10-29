# Introduction

**Rogue Pokémon** started as a collaborative school project developed by me and my good friend, [Tijl Gommers](https://be.linkedin.com/in/tijl-gommers-661572316 "LinkedIn Profile"). Over time, it evolved into a solo passion project where I built my own game engine and developed a roguelike Pokémon game inspired by Pokémon Emerald.

<div align="center">
  <img src="https://github.com/Woestijnbok/Rogue-Pokemon/blob/main/Screenshots/Thumbnail.png?raw=true" width="800" height="auto">
</div>

# Controls

Explore routes / levels, encounter wild Pokémon in tall grass, and pick up items along the way. Choose your Pokémon's moves in battle and use collected items while exploring.

The controls are designed to be simple and intuitive. A quick overview is also displayed in the console when you start the game.

- **Movement**: Use the **'ARROW'** or **'WASD'** keys to navigate around the levels.
- **Menu**: Use **'ARROW'** or **'WASD'** keys to select between your moves and **'SPACE'** to confirm any prompts.
- **Items**: Use **'H'** to use any Full restores you might have picked up, **'L'** for the Rare Candies.

# Routes

The goal of the game is to reach the highest route or level possible. To advance to the next route, you must defeat at least **50% of the wild Pokémon** in your current route.

Your **Blaziken's health and experience persist** as you progress through the game. You can gain experience by winning battles or using Rare Candies, allowing your Blaziken to grow stronger along the way.

As you progress to higher routes, the game becomes more challenging. The chance of encountering legendary Pokémon increases, and wild Pokémon become smarter when choosing which moves to use. You'll also find more grass tiles containing wild Pokémon and more tiles containing items.

You can collect items throughout each route to help you on your journey. These items are explained in the next section.

<div align="center">
  <img src="https://github.com/Woestijnbok/Rogue-Pokemon/blob/main/Screenshots/Roaming%201.png?raw=true" width="400">
  <img src="https://github.com/Woestijnbok/Rogue-Pokemon/blob/main/Screenshots/Roaming%202.png?raw=true" width="400">
</div>

# Battle System

While exploring, you may encounter wild Pokémon in tall grass that you'll have to battle using your Blaziken. You can encounter any Pokémon from the **Hoenn Pokédex**, including legendary Pokémon. You'll always start with the same Pokémon: a **Shiny Blaziken** with a fixed moveset.

I've tried to recreate the battle mechanics of Pokémon Emerald as closely as possible. Pokémon stats such as Speed, Attack, and Special Defense all play a role in determining damage and turn order.

Wild Pokémon also have an AI system that determines which move they use. Depending on their intelligence value, they will either choose a move at random or evaluate their available moves and select one based on a scoring system, generally favoring moves that deal more damage or have a type advantage.

Both the chance of encountering legendary Pokémon and the intelligence of wild Pokémon increase as you progress to higher routes.

<div align="center">
  <img src="https://github.com/Woestijnbok/Rogue-Pokemon/blob/main/Screenshots/Battle%201.png?raw=true" width="500">
  <img src="https://github.com/Woestijnbok/Rogue-Pokemon/blob/main/Screenshots/Battle%202.png?raw=true" width="500">
</div>

# Items

You can find and collect items hidden inside Poké Balls scattered throughout the different routes.

There are currently two types of items available:

- **Full Restore**: Fully restores your Blaziken's HP.
- **Rare Candy**: Increases your Blaziken's level by 1.

You can carry as many items as you find, so make sure to collect them whenever possible! Items can only be used outside of battle, using the keybinds listed in the Controls section.

<div align="center">
  <img src="https://github.com/Woestijnbok/Rogue-Pokemon/blob/main/Screenshots/Item%201.png?raw=true" width="500">
  <img src="https://github.com/Woestijnbok/Rogue-Pokemon/blob/main/Screenshots/Item%202.png?raw=true" width="500">
</div>

# Final Thoughts

I hope you enjoy playing **Rogue Pokémon** as much as I enjoyed creating it! The game combines classic Pokémon gameplay with roguelike mechanics, including randomized routes, wild Pokémon encounters, and item collection.

**P.S.** I still have plenty of ideas for new features and improvements that I'd love to implement when I return to this project!
