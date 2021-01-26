/*
 Name: Xinyue Li
 UID: 116560740
 DID: sarahl
 */

#include <stdio.h>
#include <math.h>
#define MAX 50

double compute_mean(int scores[], int size);
double compute_SD(int scores[], int size);
int *drop(int scores[], int weights[], int number_dropped, int size);
double compute_numeric(int actual_scores[], int changed_weights[], int size);

int main(){
    int i, penalty_per_day, number_dropped, number_of_a, number, score, weight,
        late, scores[MAX], weights[MAX], late_days[MAX], actural_scores[MAX],
        changed_weights[MAX], sum_of_weights = 0, *p;
    char generate;
    
    scanf("%d %d %c", &penalty_per_day, &number_dropped, &generate);
    scanf("%d", &number_of_a);
    
    /* scan the assignments' info */
    for (i = 0; i < number_of_a; i++){
        scanf("%d, %d, %d, %d", &number, &score, &weight, &late);
        scores[number - 1] = score;
        weights[number - 1] = weight;
        late_days[number - 1] = late;
    }
    
    /* check whether the sum of weights is 100 */
    for (i = 0; i < number_of_a; i++){
        sum_of_weights += weights[i];
    }
    if (sum_of_weights != 100 || number_of_a > MAX){
        printf("ERROR: Invalid values provided\n");
        return 0;
    }
    
    /* define the scores after penalty is done */
    for (i = 0; i < number_of_a; i++){
        actural_scores[i] = scores[i] - late_days[i] * penalty_per_day;
        if (actural_scores[i] < 0){
            actural_scores[i] = 0;
        }
    }
    
    /* define the scores after some assignments are dropped*/
    p = drop(scores, weights, number_dropped, number_of_a);
    for (i = 0; i < number_of_a; i++){
        changed_weights[i] = *(p + i);
    }
    
    /* output */
    printf("Numeric Score: %5.4f\n",
           compute_numeric(actural_scores, changed_weights, number_of_a));
    printf("Points Penalty Per Day Late: %d\n", penalty_per_day);
    printf("Number of Assignments Dropped: %d\n", number_dropped);
    printf("Values Provided: \n");
    printf("Assignment, Score, Weight, Days Late\n");
    for (i = 0 ; i < number_of_a; i++){
        printf("%d, %d, %d, %d\n", (i+1), scores[i], weights[i], late_days[i]);
    }
    if (generate =='Y' || generate == 'y'){
        printf("Mean: %5.4f, Standard Deviation: %5.4f\n",
               compute_mean(actural_scores,number_of_a),
               compute_SD(actural_scores, number_of_a));
    }
}

/*
 compute the mean of scores
 
 @parameter scores[]: an array of scores that need to be calculate the mean
            size: the number of the assignments
 
 @return the mean
 */
double compute_mean(int scores[], int size){
    double mean, sum = 0;
    int i;
    
    for (i = 0; i < size; i++){
        sum += scores[i];
    }
    
    mean = sum / size;
    return mean;
}

/*
 compute the mean of scores
 
 @parameter scores[]: an array of scores that need to be calculate
                      the standard deviation
            size: the number of the assignments

 @return the standard deviation
*/
double compute_SD(int scores[], int size){
    double SD, sum_of_squares = 0, mean = compute_mean(scores, size);
    int i;
    
    for (i = 0; i < size; i++){
        sum_of_squares += (scores[i] - mean) * (scores[i] - mean);
    }
    
    SD = sqrt(sum_of_squares / size);
    return SD;
}

/*
 remove the n lowest valued assignments
 1. define value as an assignment's score x the assignment's weight
 2. the number of days late and the penalty per day WILL NOT be used to decide
    what assignment to drop
 3. if two assignments have the same value, the one with the lowest assignment
    number will be dropped
 
 @parameter scores[]: an array of scores that some scores need to be dropped
            weights[]: the initial weights of each assignment
            number_dropped: the number of assignments that need to be dropped
            size: the number of the assignments
 
 @return an array of changed weights
 */
int *drop(int scores[], int weights[], int number_dropped, int size){
    int i, j, min, pos = 0, changed_weights[MAX];
    double dropped[MAX];
    
    /* get the values of each assignment */
    for (i = 0; i < size; i++){
        dropped[i] = scores[i] * weights[i] / 100;
    }
    
    /* copy weights to changed_weights */
    for (i = 0; i < size; i++){
        changed_weights[i] = weights[i];
    }
    
    /* drop assignments */
    for (i = 0; i < number_dropped; i++){
        min = 100;
        for (j = 0; j < size; j++){
            if (dropped[j] < min && changed_weights[j] != 0){
                min = dropped[j];
                pos = j;
            }
        }
        changed_weights[pos] = 0;
    }
    
    return changed_weights;
}

/*
 compute the numeric scores
 1. remove the n lowest valued assignments before performing any numeric
    score computation
 2. adjust the score for an assignment based on the number of days late and
    the points penalty per day late

 @parameter actual_scores[]: an array of scores that penalization has been done
            changed_weights[]: an array of weights after drop has been done
            size: the number of the assignments

 @return the numeric score
*/
double compute_numeric(int actual_scores[], int changed_weights[], int size){
    double numeric_score, sum = 0, sum_of_weights = 0;
    int i;
    
    for (i = 0; i < size; i++){
        sum += (double)actual_scores[i] * (double)changed_weights[i] / 100;
        sum_of_weights += changed_weights[i];
    }

    numeric_score = (double)sum / sum_of_weights * 100;
    
    return numeric_score;
}
