*sj
*g 4 1
*w
*w4 11 4 0 3
*w3 11 1 0 9
*w2 2 1 0 1
*w1 10 4 0 1
*w2 2 3 1 1
*w1 2 4 1 1
*w2 2 4 1 4
*w1 2 8 1 3
*w1 4 4 2 1
*w2 5 4 2 3
*w1 5 7 2 1
*w1 5 4 3 2
*w1 6 8 4 4
*w4 10 8 4 1
*w4 10 7 4 3
*w1 8 4 4 2
*w1 7 7 4 3
*/w
*/sj

R1 3 4 100 *sj 7 4 1 */sj
R1 0 1 50 *sj 2 2 2 */sj
R1 1 2 150 *sj 3 4 1 */sj
R1 2 4 200 *sj 7 7 3 */sj
R1 1 4 250 *sj 5 8 1 */sj
V1 2 3 15v *sj 5 4 1 */sj
V1 4 0 20v *sj 10 4 1 */sj
.end

.end



** Uitleg:
*** Alle draden staan tussen w en /w (in spicecomment) gesplitst door een comma
*** Syntax: WHoek Xcoord YCoord Node Lenght (Voor hoek: 1, wijst naar rechts, 2 naarboven, 3 naar links en 4 naar onder)
*** Voor weerstanden en bronnen: Gewoon zoals in spice.
*** Achter elke Component een spicecomment in de voor van *sj Xcoord Ycoord Hoek */sj

*** File altijd op .sj laten eindigen
