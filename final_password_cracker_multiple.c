#include <stdio.h>
#include <string.h>
#include <time.h>

# define MAX_CHARS 100

struct password_details {
    char pass[MAX_CHARS]; // assuming someones password cannot be greater
    // than 100 chars
    double time_taken;
    long int attempts;
    int bf_cracked; // cracked with the brute force algorithm
    int nbf_cracked; // cracked with the pre brute force algorithm
    short int identifier;
};

int main(void) {

    int PASSWORDS_CRACKED = 0;

    struct password_details pass1, pass2, pass3;

    printf("Please enter 3 passwords\n");
    
    scanf("%s", pass1.pass);
    scanf("%s", pass2.pass);
    scanf("%s", pass3.pass);
    
    pass1.attempts = 0;
    pass2.attempts = 0;
    pass3.attempts = 0;

    pass1.bf_cracked = 0; 
    pass2.bf_cracked = 0; 
    pass3.bf_cracked = 0;

    pass1.nbf_cracked = 0;
    pass2.nbf_cracked = 0;
    pass3.nbf_cracked = 0;

    pass1.identifier = 1; 
    pass2.identifier = 2; 
    pass3.identifier = 3;

    long int attempts = 0;

    // pre brute force algorithm 
    // txt file from https://github.com/danielmiessler/SecLists/tree/master/Passwords/Common-Credentials
    char file_pass[MAX_CHARS]; 
    FILE *fp;
    int password_found = 0;
    int len;
    int pre_bruteforce_attempts = 0;

    fp = fopen("more_common_pass.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    time_t start, end;
    double time_taken;
    start = clock();

    struct password_details pass_list[] = {pass1, pass2, pass3};

    while (fgets(file_pass, MAX_CHARS, fp) != NULL) {
        pre_bruteforce_attempts++; 
        len = strlen(file_pass);
        if (len > 0 && file_pass[len - 1] == '\n') file_pass[len-1] = '\0';
        
        for (int curr = 0; curr <= 2; curr++) {
            if (pass_list[curr].nbf_cracked != 1) {
                if (strcmp(pass_list[curr].pass, file_pass) == 0) {
                    end = clock();
                    end = end - start;
                    pass_list[curr].time_taken = ((double)end)/CLOCKS_PER_SEC;
                    printf("password %d found\n", pass_list[curr].identifier);
                    pass_list[curr].nbf_cracked = 1;
                    pass_list[curr].attempts = pre_bruteforce_attempts;
                    PASSWORDS_CRACKED++;
                }
            }
        }
        if (pass_list[0].nbf_cracked == 1 && pass_list[1].nbf_cracked == 1 && pass_list[2].nbf_cracked == 1) break;
    }
    fclose(fp);


    for (int curr = 0; curr <= 2; curr++) {
        if (pass_list[curr].nbf_cracked == 1) {
            printf("password %d took %ld attempts\n", pass_list[curr].identifier, pass_list[curr].attempts);
            printf("took %f seconds to compute\n", pass_list[curr].time_taken);
            if (pass_list[curr].attempts <= 10) {
                printf("Your password is in the top 10 most popular passwords!\n");
            } else if (pass_list[curr].attempts > 10 && pass_list[curr].attempts <= 100) {
                printf("Your password is in the top 100 most popular passwords!\n");
            } else if (pass_list[curr].attempts > 100 && pass_list[curr].attempts <= 1000) {
                printf("Your password is in the top 1000 most popular passwords!\n");
            } else if (pass_list[curr].attempts > 100 && pass_list[curr].attempts <= 10000) {
                printf("Your password is in the top 10000 most popular passwords!\n");
            } else if (pass_list[curr].attempts > 10000 && pass_list[curr].attempts <= 100000) {
                printf("Your password is in the top 10000 most popular passwords!\n");
            } else {
                printf("Your password is in the top 1000000 most popular passwords!\n");
            }

            printf("Horrible password! If hacker has access to the top 1 ");
            printf("million most common passwords, your password will be cracked ");
            printf("in under 0.1 seconds! Please do not use names or common ");
            printf("words/phrases and use a random but easy to remember ");
            printf("combinations of upper/lowercase letters and numbers ");
            printf("that are at at least 8 characters long. This type of ");
            printf("password should take 1 hour to crack.");
            printf("\n\n");
        }
    }

    if (pass_list[0].nbf_cracked == 1 && pass_list[1].nbf_cracked == 1 && pass_list[2].nbf_cracked == 1) return 0;
 
    printf("Entering brute force algorithm\n");

    char password_guesser[MAX_CHARS]; // assuming someones password cannot be greater
    // than 100 characters
    password_guesser[0] = '0';
    password_guesser[1] = '\0';
    attempts = 0;
    int curr_length = 1;
    int curr_array_elem = 0;
    int m = 0;
    int j = 0;
    int x = 0;

    while (1) {
/*
        for (j = 0; j < curr_length; j++) {
            printf("%c", password_guesser[j]);
        }
        printf("\n");
*/
        for (int curr = 0; curr <= 2; curr++) {
            if (strcmp(pass_list[curr].pass, password_guesser) == 0) {
                if (pass_list[curr].bf_cracked != 1 && pass_list[curr].nbf_cracked != 1) {
                    printf("password %d found\n", pass_list[curr].identifier);
                    end = clock();
                    end = end - start;
                    pass_list[curr].time_taken = ((double)end)/CLOCKS_PER_SEC;
                    pass_list[curr].bf_cracked = 1;
                    pass_list[curr].attempts = attempts;
                    PASSWORDS_CRACKED++;
                }
            } 
        }

        if (PASSWORDS_CRACKED == 3) {
            printf("All passwords found\n");
            break;
        }

/*
        if ((pass1.nbf_cracked == 1 || pass1.bf_cracked == 1) && (pass2.nbf_cracked == 1 || pass2.bf_cracked == 1) && (pass3.nbf_cracked == 1 || pass3.bf_cracked == 1)) {
            printf("all passwords found\n");
            break;
        }
*/

        if (password_guesser[curr_array_elem] == '9') {
            password_guesser[curr_array_elem] = 'A';
            if (curr_array_elem > 0) { // resetting 
                for (m = 0; m < curr_array_elem; m++) {
                    password_guesser[m] = '0';
                }
            }
            curr_array_elem = 0;
        } else if (password_guesser[curr_array_elem] == 'Z') {
            password_guesser[curr_array_elem] = 'a';
            if (curr_array_elem > 0) { // resetting 
                for (m = 0; m < curr_array_elem; m++) {
                    password_guesser[m] = '0';
                }
            }
            curr_array_elem = 0;
        } else if (password_guesser[curr_array_elem] == 'z') {
            curr_array_elem++;
            if (curr_array_elem == curr_length) {
                password_guesser[curr_length] = '0';
                curr_length++;
                password_guesser[curr_length] = '\0';
                for (x = 0; x < curr_length-1; x++) {
                    password_guesser[x] = '0';
                }
                curr_array_elem = 0;
            } else {
                for (m = 0; m < curr_array_elem; m++) {
                    password_guesser[m] = '0';
                }
            }
        } else {
            password_guesser[curr_array_elem]++;
            if (curr_array_elem > 0) { // resetting 
                for (m = 0; m < curr_array_elem; m++) {
                    password_guesser[m] = '0';
                }
            curr_array_elem = 0;
            }
        }
        
        attempts++;
    }    

    for (int curr = 0; curr <= 2; curr++) {
        if (pass_list[curr].bf_cracked == 1) {
            printf("Password %d took %ld attempts\n", pass_list[curr].identifier, pass_list[curr].attempts);
            printf("Took %f seconds to compute\n", pass_list[curr].time_taken);
            if (pass_list[curr].time_taken < 1) {
                printf("Horrible password! Although your password is not part of the top 1 ");
                printf("million most common passwords, it still took less than ");
                printf("1 second to crack\n");
                int len;
                for (len = 0; pass_list[curr].pass[len] != '\0'; len++);
                if (len <= 4) {
                    printf("Password is less than 5 characters long!");
                } else {
                    printf("Password is less than 6 characters long!");
                }
                printf("Please lengthen it to at least 8 characters ");
                printf("and ensure you use a combination of upper/lowercase ");
                printf("characters and numbers. Such as password should take ");
                printf("At least 1 hour to crack.");
                printf("\n\n");
            } else if (pass_list[curr].time_taken >= 1 && pass_list[curr].time_taken < 20) {
                printf("Very Poor password! Although your password is not part of the top 1 ");
                printf("million most common passwords, it still took less than ");
                printf("20 seconds to crack\n");
                int len;
                for (len = 0; pass_list[curr].pass[len] != '\0'; len++);
                if (len <= 5) {
                    printf("Password is less than 6 characters long!");
                    printf("Please lengthen it to at least 8 characters ");
                    printf("and ensure you use a combination of upper/lowercase ");
                    printf("characters and numbers. Such as password should take ");
                    printf("At least 1 hour to crack.");
                    printf("\n\n");
                }
            }

        }
    }

    return 0;
}