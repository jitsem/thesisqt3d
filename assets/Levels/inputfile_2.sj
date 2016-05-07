*sj
*c5 3
*w
*w1 3 4 0 1 666 0 0
*w2 4 3 0 1 0 0 0
*w4 1 4 2 1 0 0 1
*w1 1 4 2 1 0 0 0
*w4 1 2 1 1 0 0 0
*w3 4 1 1 1 0 0 0
*w3 3 1 1 1 0 2 0
*w3 2 1 1 1 0 0 0
*w2 4 1 1 1 0 0 0
*w1 2 4 0 1 0 0 0
*/w
*sw
*sw2 4 2 1 0
*/sw
*/sj

R1 2 1 100 *sj 1 3 4 0 50 5 */sj
V1 2 0 35v *sj 2 4 1 1 10 1 */sj
.end

*** Explanation:
*** Parameter exclusive to our application are contained withing the tags: "sj /sj". By putting these parameters in Spice-comment, we ensure some compatibility with Spice.
*** The "c" tag describes number of clicks needed for earning points in the other team's game. Maximun 5 clicks for 2 stars and maximum 3 clicks for 3 stars.
***
*** Then, all wires are contained within the tags: "w /w". They have the following sintax:
*** W[Angle] [Xcord] [YCord] [NodeNr] [Length] [Flag for connected to groud] [Flag for VoltageGoal of other team] [Flag for CurrentGoal of other team]
***
*** Switches are defined similarly:
*** sw[Angle] [Xcord] [YCord] [NodeNr1] [NodeNr2]
***
*** Resistors and Sources are defined a bit differently:
*** R(or V)[Label] [NodeNr1] [NodeNr2] [Value] *sj [Xcord] [YCord] [Angle] [Flag for adjustable] [StartingValue in game] [StepSize in Game] */sj
***
*** Files should always end with a .end statement.
*** They should also be save with the .sj extention!
