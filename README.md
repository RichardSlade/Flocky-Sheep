Flocky Sheep
==================

Author: Richard Slade 2014

A short game project designed around the use of steering behaviours and state machines to direct AI agents in a game. The agents in this case are the sheep. The player controls a sheep dog and aims to herd all the sheep to the exit before the timer reaches zero. If the player is able to herd all the sheep in the current level they are transported to a new level which has an increased size and number of sheep. Eventually the timer will reach zero and the player will be asked to play again or return to the main menu.

Steering behaviours like 'flee', 'evade', 'wander', 'flocking' etc. are used to calculate a velocity vector for each sheep and this vector used to direct them in the game world. The states are used to change these steering behaviours appropriately, for example when the dog is within a certain range the sheep change their state from relax -> runAway this in turn changes their steering behaviour to 'evade' and 'flocking' which results in the sheep evading the dog while also trying to stay close and aligned with the sheep in their immediate area.
