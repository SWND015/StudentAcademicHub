/* student_academic_hub.c
 * Integrated: partner's code + user's modules (Tasks Manager, Study Routine, Notes, Book Library)
 *
 * Compile: gcc -std=c11 -Wall -Wextra student_academic_hub.c -o student_academic_hub
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define USER_FILE "users.txt"

/* NSU-style grading table (as in partner code) */
static float nsu_grade_point(float marks) {
    if (marks >= 93.0f) return 4.00f;
    if (marks >= 90.0f) return 3.70f;
    if (marks >= 87.0f) return 3.30f;
    if (marks >= 83.0f) return 3.00f;
    if (marks >= 80.0f) return 2.70f;
    if (marks >= 77.0f) return 2.30f;
    if (marks >= 73.0f) return 2.00f;
    if (marks >= 70.0f) return 1.70f;
    if (marks >= 67.0f) return 1.30f;
    if (marks >= 60.0f) return 1.00f;
    return 0.00f;
}

static const char *nsu_grade_letter(float marks) {
    if (marks >= 93.0f) return "A";
    if (marks >= 90.0f) return "A-";
    if (marks >= 87.0f) return "B+";
    if (marks >= 83.0f) return "B";
    if (marks >= 80.0f) return "B-";
    if (marks >= 77.0f) return "C+";
    if (marks >= 73.0f) return "C";
    if (marks >= 70.0f) return "C-";
    if (marks >= 67.0f) return "D+";
    if (marks >= 60.0f) return "D";
    return "F*";
}

struct User {
    char username[50];
    char password[50];
};
struct Student {
    char name[50];
    long long id;
    float gpa;
};

// Function prototypes (partner)
int showLoginMenu(void);
void registerUser(void);
int loginUser(void);
void waitForEnter(void);

void studentResultManager(void);
void addStudentResult(void);
void viewStudentResults(void);
void searchStudentResult(void);
void deleteStudentResult(void);

void showMainMenu(void);
void cgpaTracker(void);

// Your module prototypes (implemented here)
void tasksManager(void);
void studyRoutine(void);
void notesManager(void);
void bookLibrary(void);

// ---------------------------
//            MAIN
// ---------------------------
int main(void) {
    while (1) {
        int logged = showLoginMenu();
        if (!logged) {
            // User chose to exit from login menu
            printf("\nExiting program...\n");
            break;
        }

        printf("\nLogin successful! Moving to Main Menu...\n");
        showMainMenu();
        // when showMainMenu returns, user has logged out � loop back to login
    }

    return 0;
}

// ---------------------------
//       SHOW LOGIN MENU
// ---------------------------
int showLoginMenu(void) {
    int choice;
    char buf[32];

    printf("\n==============================\n");
    printf("         LOGIN SYSTEM\n");
    printf("==============================\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");

    while (1) {
        printf("Choose option: ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        char *endptr;
        choice = (int)strtol(buf, &endptr, 10);
        if (endptr != buf && (*endptr == '\n' || *endptr == '\0')) break;
        printf("Invalid input. Please enter a number.\n");
    }

    if (choice == 1) {
        if (loginUser()) return 1; // success
        else {
            printf("\nIncorrect username or password!\n");
            waitForEnter();
            return showLoginMenu();
        }
    }
    else if (choice == 2) {
        registerUser();
        waitForEnter();
        return showLoginMenu();
    }
    else if (choice == 3) {
        printf("\nExiting program...\n");
        return 0;
    }
    else {
        printf("\nInvalid choice! Try again.\n");
        waitForEnter();
        return showLoginMenu();
    }
    return 0; // should not reach here
}

// ---------------------------
//        REGISTER USER
// ---------------------------
void registerUser() {
    struct User u;

    printf("\nEnter new username: ");
        if (fgets(u.username, sizeof(u.username), stdin) == NULL) return;
        u.username[strcspn(u.username, "\n")] = '\0';

    printf("Enter new password: ");
        if (fgets(u.password, sizeof(u.password), stdin) == NULL) return;
        u.password[strcspn(u.password, "\n")] = '\0';

    FILE *fp = fopen(USER_FILE, "a");
    if (fp == NULL) {
        printf("\nError opening file!\n");
        return;
    }

    fprintf(fp, "%s %s\n", u.username, u.password);
    fclose(fp);

    printf("\nRegistration successful!\n");
    printf("Welcome, %s! Your account has been created.\n", u.username);
}

// ---------------------------
//          LOGIN USER
// ---------------------------
int loginUser() {
    struct User u;
    char inputUser[50], inputPass[50];

    printf("\nUsername: ");
        if (fgets(inputUser, sizeof(inputUser), stdin) == NULL) return 0;
        inputUser[strcspn(inputUser, "\n")] = '\0';

    printf("Password: ");
        if (fgets(inputPass, sizeof(inputPass), stdin) == NULL) return 0;
        inputPass[strcspn(inputPass, "\n")] = '\0';

    FILE *fp = fopen(USER_FILE, "r");
    if (fp == NULL) {
        printf("\nNo users registered yet!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s", u.username, u.password) != EOF) {
        if (strcmp(inputUser, u.username) == 0 &&
            strcmp(inputPass, u.password) == 0) {

            fclose(fp);
            return 1;   // login successful
        }
    }

    fclose(fp);
    return 0;   // failed
}

// ---------------------------
//      WAIT FOR ENTER
// ---------------------------
void waitForEnter() {
    printf("\nPress ENTER to continue...");
    int c = getchar();
    if (c != '\n') { /* consume rest of line */ while ((c = getchar()) != '\n' && c != EOF) {} }
    /* wait for user to press enter */
    /* If there's already a newline, don't block further. */
}

// ================================
// Partner: Student Result Manager
// ================================
void studentResultManager() {
    int choice;
    char buf[32];

    while (1) {
        printf("\n==============================\n");
        printf("     Student Result Manager\n");
        printf("==============================\n");
        printf("1. Add Student Result\n");
        printf("2. View All Results\n");
        printf("3. Search Student Result\n");
        printf("4. Delete Record\n");
        printf("5. Back to Main Menu\n");
        printf("------------------------------\n");

        while (1) {
            printf("Enter your choice: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) return;
            char *endptr;
            choice = (int)strtol(buf, &endptr, 10);
            if (endptr != buf && (*endptr == '\n' || *endptr == '\0')) break;
            printf("Invalid input. Please enter a number.\n");
        }

        switch (choice) {
            case 1:
                addStudentResult();
                break;

            case 2:
                viewStudentResults();
                break;

            case 3:
                searchStudentResult();
                break;

            case 4:
                deleteStudentResult();
                break;

            case 5:
                printf("\nReturning to Main Menu...\n");
                return;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
}

void addStudentResult() {
    struct Student s;
    FILE *fp;

    fp = fopen("student_results.txt", "a");
    if (fp == NULL) {
        printf("\nError opening file!\n");
        return;
    }

    printf("\n===== Add Student Result =====\n");

    printf("Enter Student Name: ");
    int _ch = getchar();
    if (_ch != '\n' && _ch != EOF) {
        ungetc(_ch, stdin);
    }
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Student ID: ");
    if (scanf("%lld", &s.id) != 1) {
        int _c; while ((_c = getchar()) != '\n' && _c != EOF) {}
        printf("\nInvalid ID input. Aborting add.\n");
        fclose(fp);
        return;
    }

    printf("Enter number of subjects: ");
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("\nInvalid number of subjects. Aborting add.\n");
        fclose(fp);
        return;
    }

    int c; while ((c = getchar()) != '\n' && c != EOF) {}

    char **subj = malloc(sizeof(char*) * n);
    float *marks = malloc(sizeof(float) * n);
    if (!subj || !marks) {
        printf("\nMemory allocation failed.\n");
        free(subj); free(marks);
        fclose(fp);
        return;
    }

    float total_marks = 0;
    float total_grade_points = 0;
    for (int i = 0; i < n; i++) {
        subj[i] = malloc(64);
        if (!subj[i]) subj[i] = NULL;
        printf("Enter name for Subject %d: ", i + 1);
        if (fgets(subj[i], 64, stdin) != NULL) {
            subj[i][strcspn(subj[i], "\n")] = '\0';
        }
        printf("Enter marks for %s: ", subj[i]);
        if (scanf("%f", &marks[i]) != 1) marks[i] = 0.0f;
        while ((c = getchar()) != '\n' && c != EOF) {}

        total_marks += marks[i];
        total_grade_points += nsu_grade_point(marks[i]);
    }

    s.gpa = (n > 0) ? (total_grade_points / (float)n) : 0.0f;

    {
        char choicebuf[8];
        printf("\nComputed GPA (from marks): %.2f\n", s.gpa);
        printf("Use this GPA? (Y/n): ");
        if (fgets(choicebuf, sizeof(choicebuf), stdin) != NULL) {
            if (choicebuf[0] == 'n' || choicebuf[0] == 'N') {
                char gbuf[64];
                double manual = -1.0;
                while (1) {
                    printf("Enter GPA manually (0.00 - 4.00): ");
                    if (fgets(gbuf, sizeof(gbuf), stdin) == NULL) break;
                    char *endptr; manual = strtod(gbuf, &endptr);
                    if (endptr != gbuf && (*endptr == '\n' || *endptr == '\0') && manual >= 0.0 && manual <= 4.0) {
                        s.gpa = (float)manual;
                        break;
                    }
                    printf("Invalid GPA. Try again.\n");
                }
            }
        }
    }

    fprintf(fp, "%lld|%s|%d|%.2f|", s.id, s.name, n, s.gpa);
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s,%.2f;", subj[i] ? subj[i] : "", marks[i]);
    }
    fprintf(fp, "\n");

    fclose(fp);

    printf("\nStudent Result Added Successfully!\n");

    for (int i = 0; i < n; i++) {
        if (subj[i]) free(subj[i]);
    }
    free(subj);
    free(marks);
}

void viewStudentResults() {
    FILE *fp = fopen("student_results.txt", "r");
    if (fp == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    char line[8192];
    printf("\n===== All Student Results =====\n");
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *p = strtok(line, "|\n");
        if (!p) continue;
        long long id = strtoll(p, NULL, 10);
        char *name = strtok(NULL, "|\n");
        char *nstr = strtok(NULL, "|\n");
        (void)nstr;
        char *gpaStr = strtok(NULL, "|\n");
        char *rest = strtok(NULL, "\n");
        float gpa = gpaStr ? atof(gpaStr) : 0.0f;

        printf("\nID: %lld\n", id);
        printf("Name: %s\n", name ? name : "");
        printf("GPA: %.2f\n", gpa);
        printf("Subjects:\n");
        if (rest) {
            char *pair = strtok(rest, ";");
            while (pair != NULL) {
                char *comma = strchr(pair, ',');
                if (comma) {
                    *comma = '\0';
                    char *sname = pair;
                    float mark = atof(comma + 1);
                    const char *letter = nsu_grade_letter(mark);
                    float gp = nsu_grade_point(mark);
                    printf(" - %s : %.2f (%s, %.2f)\n", sname, mark, letter, gp);
                }
                pair = strtok(NULL, ";");
            }
        }
    }

    fclose(fp);
    waitForEnter();
}

void searchStudentResult() {
    long long searchID;
    int found = 0;

    FILE *fp = fopen("student_results.txt", "r");
    if (fp == NULL) {
        printf("\nNo records found!\n");
        return;
    }

    printf("\nEnter Student ID to search: ");
    char idbuf[64];
    if (fgets(idbuf, sizeof(idbuf), stdin) == NULL) {
        if (scanf("%lld", &searchID) != 1) {
            int _c; while ((_c = getchar()) != '\n' && _c != EOF) {}
            printf("\nInvalid ID input.\n");
            return;
        }
    } else {
        char *endptr;
        searchID = strtoll(idbuf, &endptr, 10);
        if (endptr == idbuf || (*endptr != '\n' && *endptr != '\0')) {
            printf("\nInvalid ID input.\n");
            return;
        }
    }

    char line[8192];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char copy[8192]; strncpy(copy, line, sizeof(copy));
        char *p = strtok(copy, "|\n");
        if (!p) continue;
        long long id = strtoll(p, NULL, 10);
        if (id != searchID) continue;

        char *name = strtok(NULL, "|\n");
        char *nstr = strtok(NULL, "|\n");
        (void)nstr;
        char *gpaStr = strtok(NULL, "|\n");
        char *rest = strtok(NULL, "\n");

        printf("\n=== Student Found ===\n");
        printf("ID: %lld\n", id);
        printf("Name: %s\n", name ? name : "");
        printf("GPA: %.2f\n", gpaStr ? atof(gpaStr) : 0.0f);
        printf("Subjects:\n");
        if (rest) {
            char *pair = strtok(rest, ";");
            while (pair != NULL) {
                char *comma = strchr(pair, ',');
                if (comma) {
                    *comma = '\0';
                    char *sname = pair;
                    float mark = atof(comma + 1);
                    printf(" - %s : %.2f\n", sname, mark);
                }
                pair = strtok(NULL, ";");
            }
        }
        found = 1;
        break;
    }

    if (!found)
        printf("\nStudent not found!\n");

    fclose(fp);
    waitForEnter();
}

void deleteStudentResult() {
    long long deleteID; int found = 0;

    FILE *fp = fopen("student_results.txt", "r");
    if (fp == NULL) {
        printf("\nNo records to delete!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("\nError creating temporary file!\n");
        fclose(fp);
        return;
    }

    printf("\nEnter Student ID to delete: ");
    char idbuf[64];
    if (fgets(idbuf, sizeof(idbuf), stdin) == NULL) {
        if (scanf("%lld", &deleteID) != 1) {
            int _c; while ((_c = getchar()) != '\n' && _c != EOF) {}
            printf("\nInvalid ID input.\n");
            fclose(fp); fclose(temp);
            return;
        }
    } else {
        char *endptr;
        deleteID = strtoll(idbuf, &endptr, 10);
        if (endptr == idbuf || (*endptr != '\n' && *endptr != '\0')) {
            printf("\nInvalid ID input.\n");
            fclose(fp); fclose(temp);
            return;
        }
    }

    char line[8192];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char copy[8192]; strncpy(copy, line, sizeof(copy));
        char *p = strtok(copy, "|\n");
        if (!p) continue;
        long long id = strtoll(p, NULL, 10);
        if (id == deleteID) {
            found = 1;
            continue; // skip writing this line
        }
        fputs(line, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("student_results.txt");
    rename("temp.txt", "student_results.txt");

    if (found) {
        printf("\nRecord deleted successfully!\n");
    } else {
        printf("\nStudent not found!\n");
    }
    waitForEnter();
}

// ---------------------------
//       MAIN MENU / MODULES
// ---------------------------
void showMainMenu(void) {
    int choice;
    char buf[32];

    while (1) {
        printf("\n==============================\n");
        printf("           MAIN MENU\n");
        printf("==============================\n");
        printf("1. Student Result Manager\n");
        printf("2. CGPA Tracker\n");
        printf("3. Tasks Manager\n");
        printf("4. Study Routine\n");
        printf("5. Book Library\n");
        printf("6. Notes\n");
        printf("7. Logout\n");

        while (1) {
            printf("Choose option: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) return;
            char *endptr;
            choice = (int)strtol(buf, &endptr, 10);
            if (endptr != buf && (*endptr == '\n' || *endptr == '\0')) break;
            printf("Invalid input. Please enter a number.\n");
        }

        switch (choice) {
            case 1:
                studentResultManager();
                break;
            case 2:
                cgpaTracker();
                break;
            case 3:
                tasksManager();
                break;
            case 4:
                studyRoutine();
                break;
            case 5:
                bookLibrary();
                break;
            case 6:
                notesManager();
                break;
            case 7:
                printf("\nLogging out...\n");
                return; // return to login
            default:
                printf("\nInvalid choice. Try again.\n");
        }
    }
}

// ---------------------------
// Partner: CGPA Tracker
// ---------------------------
void cgpaTracker(void) {
    int choice;
    char buf[64];

    while (1) {
        printf("\n==============================\n");
        printf("         CGPA Tracker\n");
        printf("==============================\n");
        printf("1. List stored GPAs (from records)\n");
        printf("2. Compute CGPA from semester GPAs\n");
        printf("3. Back\n");

        while (1) {
            printf("Choose option: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) return;
            char *endptr;
            choice = (int)strtol(buf, &endptr, 10);
            if (endptr != buf && (*endptr == '\n' || *endptr == '\0')) break;
            printf("Invalid input. Please enter a number.\n");
        }

        if (choice == 1) {
            FILE *fp = fopen("student_results.txt", "r");
            if (fp == NULL) {
                printf("\nNo records found!\n");
                waitForEnter();
                continue;
            }

            char line[8192];
            int any = 0;
            printf("\nStored Student GPAs:\n");
            printf("%-12s  %-20s  %-4s\n", "ID", "Name", "GPA");
            while (fgets(line, sizeof(line), fp) != NULL) {
                char copy[8192]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|");
                if (!p) continue;
                long long id = strtoll(p, NULL, 10);
                char *name = strtok(NULL, "|");
                char *nstr = strtok(NULL, "|"); /* skip/inspect n if needed */
                (void)nstr;
                char *gpaStr = strtok(NULL, "|");
                float gpa = gpaStr ? atof(gpaStr) : 0.0f;
                if (name) name[strcspn(name, "\n")] = '\0';
                printf("%-12lld  %-20s  %.2f\n", id, name ? name : "", gpa);
                any = 1;
            }
            fclose(fp);
            if (!any) printf("(no records)\n");
            waitForEnter();

        } else if (choice == 2) {
            int semesters = 0;
            while (1) {
                printf("Enter number of semesters: ");
                if (fgets(buf, sizeof(buf), stdin) == NULL) return;
                char *endptr;
                semesters = (int)strtol(buf, &endptr, 10);
                if (endptr != buf && (*endptr == '\n' || *endptr == '\0') && semesters > 0) break;
                printf("Invalid number. Please enter a positive integer.\n");
            }

            double sum = 0.0;
            for (int i = 1; i <= semesters; ++i) {
                double g;
                while (1) {
                    printf("Enter GPA for semester %d (0.00 - 4.00): ", i);
                    if (fgets(buf, sizeof(buf), stdin) == NULL) return;
                    char *endptr;
                    g = strtod(buf, &endptr);
                    if (endptr != buf && (*endptr == '\n' || *endptr == '\0') && g >= 0.0 && g <= 4.0) break;
                    printf("Invalid GPA. Enter a number between 0 and 4.\n");
                }
                sum += g;
            }
            double cgpa = sum / semesters;
            printf("\nComputed CGPA: %.2f\n", cgpa);
            waitForEnter();

        } else if (choice == 3) {
            return;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }
}

// ---------------------------
// YOUR MODULES IMPLEMENTATION
// ---------------------------

/* TASKS MANAGER
 * Format stored in tasks.txt:
 * id|title|description|due_date|status\n
 * status: 0 = pending, 1 = done
 */
static long long next_id_from_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 1;
    long long max = 0;
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        char *p = strtok(line, "|");
        if (!p) continue;
        long long id = strtoll(p, NULL, 10);
        if (id > max) max = id;
    }
    fclose(fp);
    return max + 1;
}

void tasksManager(void) {
    int choice;
    char buf[256];

    while (1) {
        printf("\n==============================\n");
        printf("         Tasks Manager\n");
        printf("==============================\n");
        printf("1. Add Task\n");
        printf("2. List Tasks (filter/search)\n");
        printf("3. Mark Task As Done\n");
        printf("4. Delete Task\n");
        printf("5. Search Tasks\n");
        printf("6. Back to Main Menu\n");

        printf("Choose option: ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) return;
        char *endptr;
        choice = (int)strtol(buf, &endptr, 10);
        if (endptr == buf || (*endptr != '\n' && *endptr != '\0')) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if (choice == 1) {
            char title[256], desc[512], due[64];
            int priority = 1; /* 1=High,2=Medium,3=Low */

            printf("Enter task title: ");
            if (fgets(title, sizeof(title), stdin) == NULL) continue;
            title[strcspn(title, "\n")] = '\0';
            if (strlen(title) == 0) { printf("Title cannot be empty.\n"); continue; }

            printf("Enter task description: ");
            if (fgets(desc, sizeof(desc), stdin) == NULL) continue;
            desc[strcspn(desc, "\n")] = '\0';

            printf("Enter due date (optional, YYYY-MM-DD): ");
            if (fgets(due, sizeof(due), stdin) == NULL) continue;
            due[strcspn(due, "\n")] = '\0';

            while (1) {
                char pbuf[8];
                printf("Priority (1=High,2=Medium,3=Low) [2]: ");
                if (fgets(pbuf, sizeof(pbuf), stdin) == NULL) break;
                if (pbuf[0] == '\n') { priority = 2; break; }
                char *ep; int pv = (int)strtol(pbuf, &ep, 10);
                if (ep != pbuf && (*ep == '\n' || *ep == '\0') && pv >= 1 && pv <= 3) { priority = pv; break; }
                printf("Invalid priority. Enter 1, 2 or 3.\n");
            }

            long long id = next_id_from_file("tasks.txt");
            FILE *fp = fopen("tasks.txt", "a");
            if (!fp) { printf("Error opening tasks file.\n"); continue; }
            /* New format: id|title|desc|due|priority|status\n */
            fprintf(fp, "%lld|%s|%s|%s|%d|%d\n", id, title, desc, due, priority, 0);
            fclose(fp);
            printf("Task added (ID: %lld).\n", id);
            waitForEnter();

        } else if (choice == 2) {
            /* List with optional filter */
            printf("\nList options:\n");
            printf("1. All\n2. Pending only\n3. Done only\n4. By priority\n");
            printf("Choose: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
            int listOpt = (int)strtol(buf, &endptr, 10);
            int filterStatus = -1; /* -1 = all, 0 pending, 1 done */
            int filterPriority = 0; /* 0 = any, 1/2/3 */
            if (listOpt == 2) filterStatus = 0;
            else if (listOpt == 3) filterStatus = 1;
            else if (listOpt == 4) {
                printf("Priority to show (1=High,2=Medium,3=Low): ");
                if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
                filterPriority = (int)strtol(buf, &endptr, 10);
                if (filterPriority < 1 || filterPriority > 3) filterPriority = 0;
            }

            FILE *fp = fopen("tasks.txt", "r");
            if (!fp) { printf("\nNo tasks found.\n"); waitForEnter(); continue; }
            printf("\nYour Tasks:\n");
            printf("%-6s  %-20s  %-10s  %-8s  %-8s\n", "ID", "Title", "Due", "Priority", "Status");
            char line[2048];
            int any = 0;
            while (fgets(line, sizeof(line), fp)) {
                char copy[2048]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|\n"); if (!p) continue;
                long long id = strtoll(p, NULL, 10);
                char *title = strtok(NULL, "|\n");
                char *desc = strtok(NULL, "|\n");
                (void)desc;
                char *due = strtok(NULL, "|\n");
                char *priorityStr = strtok(NULL, "|\n");
                char *statusStr = strtok(NULL, "|\n");
                int pr = priorityStr ? atoi(priorityStr) : 2;
                int st = statusStr ? atoi(statusStr) : 0;
                if ((filterStatus != -1 && st != filterStatus) || (filterPriority != 0 && pr != filterPriority)) continue;
                const char *prname = (pr == 1) ? "High" : (pr == 2 ? "Med" : "Low");
                printf("%-6lld  %-20s  %-10s  %-8s  %-8s\n", id, title ? title : "", due ? due : "", prname, st ? "Done" : "Pending");
                any = 1;
            }
            fclose(fp);
            if (!any) printf("(no matching tasks)\n");
            waitForEnter();

        } else if (choice == 3) {
            printf("Enter Task ID to mark done: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
            long long id = strtoll(buf, NULL, 10);
            FILE *fp = fopen("tasks.txt", "r");
            if (!fp) { printf("No tasks file.\n"); waitForEnter(); continue; }
            FILE *tmp = fopen("tmp_tasks.txt", "w");
            if (!tmp) { fclose(fp); printf("Error.\n"); continue; }
            char line[2048]; int found = 0;
            while (fgets(line, sizeof(line), fp)) {
                char copy[2048]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|\n"); if (!p) continue;
                long long cur = strtoll(p, NULL, 10);
                char *title = strtok(NULL, "|\n");
                char *desc = strtok(NULL, "|\n");
                char *due = strtok(NULL, "|\n");
                char *priorityStr = strtok(NULL, "|\n");
                char *statusStr = strtok(NULL, "|\n");
                int pr = priorityStr ? atoi(priorityStr) : 2;
                int st = statusStr ? atoi(statusStr) : 0;
                if (cur == id) {
                    found = 1;
                    if (st == 1) {
                        fprintf(tmp, "%lld|%s|%s|%s|%d|%d\n", cur, title?title:"", desc?desc:"", due?due:"", pr, st);
                    } else {
                        fprintf(tmp, "%lld|%s|%s|%s|%d|%d\n", cur, title?title:"", desc?desc:"", due?due:"", pr, 1);
                    }
                } else {
                    fputs(line, tmp);
                }
            }
            fclose(fp); fclose(tmp);
            if (found) {
                remove("tasks.txt");
                rename("tmp_tasks.txt", "tasks.txt");
                printf("Task updated (marked done if it was pending).\n");
            } else {
                remove("tmp_tasks.txt");
                printf("Task ID not found.\n");
            }
            waitForEnter();

        } else if (choice == 4) {
            printf("Enter Task ID to delete: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
            long long id = strtoll(buf, NULL, 10);
            FILE *fp = fopen("tasks.txt", "r");
            if (!fp) { printf("No tasks file.\n"); waitForEnter(); continue; }
            FILE *tmp = fopen("tmp_tasks.txt", "w");
            if (!tmp) { fclose(fp); printf("Error.\n"); continue; }
            char line[2048]; int found = 0;
            while (fgets(line, sizeof(line), fp)) {
                char copy[2048]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|\n"); if (!p) continue;
                long long cur = strtoll(p, NULL, 10);
                if (cur == id) { found = 1; continue; }
                fputs(line, tmp);
            }
            fclose(fp); fclose(tmp);
            if (found) {
                remove("tasks.txt");
                rename("tmp_tasks.txt", "tasks.txt");
                printf("Task deleted.\n");
            } else {
                remove("tmp_tasks.txt");
                printf("Task ID not found.\n");
            }
            waitForEnter();

        } else if (choice == 5) {
            printf("Enter keyword to search title/description: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
            buf[strcspn(buf, "\n")] = '\0';
            FILE *fp = fopen("tasks.txt", "r");
            if (!fp) { printf("No tasks file.\n"); waitForEnter(); continue; }
            char line[2048]; int any = 0;
            while (fgets(line, sizeof(line), fp)) {
                char copy[2048]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|\n"); if (!p) continue;
                long long id = strtoll(p, NULL, 10);
                char *title = strtok(NULL, "|\n");
                char *desc = strtok(NULL, "|\n");
                char *due = strtok(NULL, "|\n");
                char *priorityStr = strtok(NULL, "|\n");
                char *statusStr = strtok(NULL, "|\n");
                int pr = priorityStr ? atoi(priorityStr) : 2;
                int st = statusStr ? atoi(statusStr) : 0;
                if ((title && strstr(title, buf)) || (desc && strstr(desc, buf))) {
                    const char *prname = (pr == 1) ? "High" : (pr == 2 ? "Med" : "Low");
                    printf("ID:%-6lld  %s  Due:%-10s  P:%-3s  %s\n", id, title?title:"", due?due:"", prname, st?"Done":"Pending");
                    any = 1;
                }
            }
            fclose(fp);
            if (!any) printf("No matching tasks.\n");
            waitForEnter();

        } else if (choice == 6) {
            return;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* STUDY ROUTINE
 * routines.txt lines:
 * id|title|day_or_date|time|notes\n
 */
void studyRoutine(void) {
    char buf[256];
    int choice;
    while (1) {
        printf("\n==============================\n");
        printf("         Study Routine\n");
        printf("==============================\n");
        printf("1. Add Routine Item\n");
        printf("2. View Routine\n");
        printf("3. Delete Routine Item\n");
        printf("4. Back to Main Menu\n");
        printf("Choose option: ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) return;
        char *endptr;
        choice = (int)strtol(buf, &endptr, 10);
        if (endptr == buf || (*endptr != '\n' && *endptr != '\0')) { printf("Invalid input.\n"); continue; }

        if (choice == 1) {
            char title[256], day[64], time[64], notes[512];
            printf("Enter title (e.g. 'Math practice'): ");
            if (fgets(title, sizeof(title), stdin) == NULL) continue;
            title[strcspn(title, "\n")] = '\0';
            printf("Enter day/date (e.g. Monday or 2025-12-31): ");
            if (fgets(day, sizeof(day), stdin) == NULL) continue;
            day[strcspn(day, "\n")] = '\0';
            printf("Enter time (e.g. 18:00): ");
            if (fgets(time, sizeof(time), stdin) == NULL) continue;
            time[strcspn(time, "\n")] = '\0';
            printf("Optional notes: ");
            if (fgets(notes, sizeof(notes), stdin) == NULL) continue;
            notes[strcspn(notes, "\n")] = '\0';

            long long id = next_id_from_file("routines.txt");
            FILE *fp = fopen("routines.txt", "a");
            if (!fp) { printf("Error opening routines file.\n"); continue; }
            fprintf(fp, "%lld|%s|%s|%s|%s\n", id, title, day, time, notes);
            fclose(fp);
            printf("Routine item added (ID %lld).\n", id);
            waitForEnter();

        } else if (choice == 2) {
            FILE *fp = fopen("routines.txt", "r");
            if (!fp) { printf("\nNo routines found.\n"); waitForEnter(); continue; }
            printf("\nYour Study Routine:\n");
            printf("%-6s  %-25s  %-12s  %-8s\n", "ID", "Title", "Day/Date", "Time");
            char line[2048];
            while (fgets(line, sizeof(line), fp)) {
                char *p = strtok(line, "|\n");
                if (!p) continue;
                long long id = strtoll(p, NULL, 10);
                char *title = strtok(NULL, "|\n");
                char *day = strtok(NULL, "|\n");
                char *time = strtok(NULL, "|\n");
                char *notes = strtok(NULL, "|\n");
                printf("%-6lld  %-25s  %-12s  %-8s\n", id, title ? title : "", day ? day : "", time ? time : "");
                if (notes && strlen(notes) > 0) printf("    Notes: %s\n", notes);
            }
            fclose(fp);
            waitForEnter();

        } else if (choice == 3) {
            printf("Enter Routine ID to delete: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
            long long id = strtoll(buf, NULL, 10);
            FILE *fp = fopen("routines.txt", "r");
            if (!fp) { printf("No routines file.\n"); waitForEnter(); continue; }
            FILE *tmp = fopen("tmp_routines.txt", "w");
            if (!tmp) { fclose(fp); printf("Error.\n"); continue; }
            char line[2048]; int found = 0;
            while (fgets(line, sizeof(line), fp)) {
                char copy[2048]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|\n");
                if (!p) continue;
                long long cur = strtoll(p, NULL, 10);
                if (cur == id) { found = 1; continue; }
                fputs(line, tmp);
            }
            fclose(fp); fclose(tmp);
            if (found) {
                remove("routines.txt");
                rename("tmp_routines.txt", "routines.txt");
                printf("Routine item deleted.\n");
            } else {
                remove("tmp_routines.txt");
                printf("Routine ID not found.\n");
            }
            waitForEnter();

        } else if (choice == 4) {
            return;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* NOTES MANAGER (Google Notes like)
 * notes.txt format:
 * id|title|content|color_tag(optional)\n
 */
void notesManager(void) {
    char buf[512];
    int choice;
    while (1) {
        printf("\n==============================\n");
        printf("            Notes\n");
        printf("==============================\n");
        printf("1. Add Note\n");
        printf("2. List Notes\n");
        printf("3. View Note\n");
        printf("4. Edit Note\n");
        printf("5. Delete Note\n");
        printf("6. Back to Main Menu\n");
        printf("Choose option: ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) return;
        char *endptr;
        choice = (int)strtol(buf, &endptr, 10);
        if (endptr == buf || (*endptr != '\n' && *endptr != '\0')) { printf("Invalid input.\n"); continue; }

        if (choice == 1) {
            char title[256], content[2048], tag[64];
            printf("Enter note title: ");
            if (fgets(title, sizeof(title), stdin) == NULL) continue;
            title[strcspn(title, "\n")] = '\0';
            printf("Enter content (single line supported): ");
            if (fgets(content, sizeof(content), stdin) == NULL) continue;
            content[strcspn(content, "\n")] = '\0';
            printf("Optional color/tag (e.g. yellow): ");
            if (fgets(tag, sizeof(tag), stdin) == NULL) continue;
            tag[strcspn(tag, "\n")] = '\0';

            long long id = next_id_from_file("notes.txt");
            FILE *fp = fopen("notes.txt", "a");
            if (!fp) { printf("Error opening notes file.\n"); continue; }
            fprintf(fp, "%lld|%s|%s|%s\n", id, title, content, tag);
            fclose(fp);
            printf("Note added (ID %lld).\n", id);
            waitForEnter();

        } else if (choice == 2) {
            FILE *fp = fopen("notes.txt", "r");
            if (!fp) { printf("\nNo notes found.\n"); waitForEnter(); continue; }
            printf("\nNotes:\n");
            printf("%-6s  %-30s  %-10s\n", "ID", "Title", "Tag");
            char line[4096];
            while (fgets(line, sizeof(line), fp)) {
                char *p = strtok(line, "|\n");
                if (!p) continue;
                long long id = strtoll(p, NULL, 10);
                char *title = strtok(NULL, "|\n");
                char *content = strtok(NULL, "|\n");
                (void)content;
                char *tag = strtok(NULL, "|\n");
                printf("%-6lld  %-30s  %-10s\n", id, title ? title : "", tag ? tag : "");
            }
            fclose(fp);
            waitForEnter();

        } else if (choice == 3) {
            printf("Enter Note ID to view: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
            long long id = strtoll(buf, NULL, 10);
            FILE *fp = fopen("notes.txt", "r");
            if (!fp) { printf("No notes file.\n"); waitForEnter(); continue; }
            char line[4096]; int found = 0;
            while (fgets(line, sizeof(line), fp)) {
                char copy[4096]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|\n");
                if (!p) continue;
                long long cur = strtoll(p, NULL, 10);
                if (cur == id) {
                    found = 1;
                    char *title = strtok(NULL, "|\n");
                    char *content = strtok(NULL, "|\n");
                    char *tag = strtok(NULL, "|\n");
                    printf("\nNote ID: %lld\nTitle: %s\nTag: %s\nContent:\n%s\n", cur, title ? title : "", tag ? tag : "", content ? content : "");
                    break;
                }
            }
            fclose(fp);
            if (!found) printf("Note not found.\n");
            waitForEnter();

        } else if (choice == 4) {
            printf("Enter Note ID to edit: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
            long long id = strtoll(buf, NULL, 10);
            FILE *fp = fopen("notes.txt", "r");
            if (!fp) { printf("No notes file.\n"); waitForEnter(); continue; }
            FILE *tmp = fopen("tmp_notes.txt", "w");
            if (!tmp) { fclose(fp); printf("Error.\n"); continue; }
            char line[4096]; int found = 0;
            while (fgets(line, sizeof(line), fp)) {
                char copy[4096]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|\n");
                if (!p) continue;
                long long cur = strtoll(p, NULL, 10);
                if (cur == id) {
                    found = 1;
                    char *title = strtok(NULL, "|\n");
                    char *content = strtok(NULL, "|\n");
                    char *tag = strtok(NULL, "|\n");
                    char newTitle[256], newContent[2048], newTag[64];
                    printf("Editing Note ID %lld\n", id);
                    printf("Current title: %s\nEnter new title (leave empty to keep): ", title ? title : "");
                    if (fgets(newTitle, sizeof(newTitle), stdin) == NULL) newTitle[0]='\0';
                    newTitle[strcspn(newTitle, "\n")] = '\0';
                    printf("Current content: %s\nEnter new content (leave empty to keep): ", content ? content : "");
                    if (fgets(newContent, sizeof(newContent), stdin) == NULL) newContent[0]='\0';
                    newContent[strcspn(newContent, "\n")] = '\0';
                    printf("Current tag: %s\nEnter new tag (leave empty to keep): ", tag ? tag : "");
                    if (fgets(newTag, sizeof(newTag), stdin) == NULL) newTag[0]='\0';
                    newTag[strcspn(newTag, "\n")] = '\0';

                    if (strlen(newTitle) == 0) strncpy(newTitle, title ? title : "", sizeof(newTitle));
                    if (strlen(newContent) == 0) strncpy(newContent, content ? content : "", sizeof(newContent));
                    if (strlen(newTag) == 0) strncpy(newTag, tag ? tag : "", sizeof(newTag));

                    fprintf(tmp, "%lld|%s|%s|%s\n", cur, newTitle, newContent, newTag);
                } else {
                    fputs(line, tmp);
                }
            }
            fclose(fp); fclose(tmp);
            if (found) {
                remove("notes.txt");
                rename("tmp_notes.txt", "notes.txt");
                printf("Note updated.\n");
            } else {
                remove("tmp_notes.txt");
                printf("Note not found.\n");
            }
            waitForEnter();

        } else if (choice == 5) {
            printf("Enter Note ID to delete: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
            long long id = strtoll(buf, NULL, 10);
            FILE *fp = fopen("notes.txt", "r");
            if (!fp) { printf("No notes file.\n"); waitForEnter(); continue; }
            FILE *tmp = fopen("tmp_notes.txt", "w");
            if (!tmp) { fclose(fp); printf("Error.\n"); continue; }
            char line[4096]; int found = 0;
            while (fgets(line, sizeof(line), fp)) {
                char copy[4096]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|\n");
                if (!p) continue;
                long long cur = strtoll(p, NULL, 10);
                if (cur == id) { found = 1; continue; }
                fputs(line, tmp);
            }
            fclose(fp); fclose(tmp);
            if (found) {
                remove("notes.txt");
                rename("tmp_notes.txt", "notes.txt");
                printf("Note deleted.\n");
            } else {
                remove("tmp_notes.txt");
                printf("Note not found.\n");
            }
            waitForEnter();

        } else if (choice == 6) {
            return;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* BOOK LIBRARY
 * books.txt format:
 * id|title|author|year|status\n
 * status: available / borrowed
 */
void bookLibrary(void) {
    char buf[512];
    int choice;
    while (1) {
        printf("\n==============================\n");
        printf("         Book Library\n");
        printf("==============================\n");
        printf("1. Add Book\n");
        printf("2. List Books\n");
        printf("3. Search Book\n");
        printf("4. Borrow Book\n");
        printf("5. Return Book\n");
        printf("6. Delete Book\n");
        printf("7. Back to Main Menu\n");
        printf("Choose option: ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) return;
        char *endptr;
        choice = (int)strtol(buf, &endptr, 10);
        if (endptr == buf || (*endptr != '\n' && *endptr != '\0')) { printf("Invalid input.\n"); continue; }

        if (choice == 1) {
            char title[256], author[256], year[16];
            printf("Enter book title: ");
            if (fgets(title, sizeof(title), stdin) == NULL) continue;
            title[strcspn(title, "\n")] = '\0';
            printf("Enter author: ");
            if (fgets(author, sizeof(author), stdin) == NULL) continue;
            author[strcspn(author, "\n")] = '\0';
            printf("Enter year: ");
            if (fgets(year, sizeof(year), stdin) == NULL) continue;
            year[strcspn(year, "\n")] = '\0';

            long long id = next_id_from_file("books.txt");
            FILE *fp = fopen("books.txt", "a");
            if (!fp) { printf("Error opening books file.\n"); continue; }
            fprintf(fp, "%lld|%s|%s|%s|available\n", id, title, author, year);
            fclose(fp);
            printf("Book added (ID %lld).\n", id);
            waitForEnter();

        } else if (choice == 2) {
            FILE *fp = fopen("books.txt", "r");
            if (!fp) { printf("\nNo books found.\n"); waitForEnter(); continue; }
            printf("\nBooks:\n");
            printf("%-6s  %-30s  %-20s  %-6s  %-10s\n", "ID", "Title", "Author", "Year", "Status");
            char line[2048];
            while (fgets(line, sizeof(line), fp)) {
                char *p = strtok(line, "|\n");
                if (!p) continue;
                long long id = strtoll(p, NULL, 10);
                char *title = strtok(NULL, "|\n");
                char *author = strtok(NULL, "|\n");
                char *year = strtok(NULL, "|\n");
                char *status = strtok(NULL, "|\n");
                printf("%-6lld  %-30s  %-20s  %-6s  %-10s\n", id, title ? title : "", author ? author : "", year ? year : "", status ? status : "");
            }
            fclose(fp);
            waitForEnter();

        } else if (choice == 3) {
            printf("Enter keyword to search title/author: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
            buf[strcspn(buf, "\n")] = '\0';
            FILE *fp = fopen("books.txt", "r");
            if (!fp) { printf("No books file.\n"); waitForEnter(); continue; }
            char line[2048]; int any = 0;
            while (fgets(line, sizeof(line), fp)) {
                char copy[2048]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|\n");
                if (!p) continue;
                long long id = strtoll(p, NULL, 10);
                char *title = strtok(NULL, "|\n");
                char *author = strtok(NULL, "|\n");
                char *year = strtok(NULL, "|\n");
                char *status = strtok(NULL, "|\n");
                if ((title && strstr(title, buf)) || (author && strstr(author, buf))) {
                    if (!any) printf("\nSearch Results:\n");
                    any = 1;
                    printf("%-6lld  %-30s  %-20s  %-6s  %-10s\n", id, title ? title : "", author ? author : "", year ? year : "", status ? status : "");
                }
            }
            fclose(fp);
            if (!any) printf("No matching books.\n");
            waitForEnter();

        } else if (choice == 4 || choice == 5) {
            bool borrow = (choice == 4);
            printf("%s - Enter Book ID: ", borrow ? "Borrow" : "Return");
            if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
            long long id = strtoll(buf, NULL, 10);
            FILE *fp = fopen("books.txt", "r");
            if (!fp) { printf("No books file.\n"); waitForEnter(); continue; }
            FILE *tmp = fopen("tmp_books.txt", "w");
            if (!tmp) { fclose(fp); printf("Error.\n"); continue; }
            char line[2048]; int found = 0; int updated = 0;
            while (fgets(line, sizeof(line), fp)) {
                char copy[2048]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|\n");
                if (!p) continue;
                long long cur = strtoll(p, NULL, 10);
                char *title = strtok(NULL, "|\n");
                char *author = strtok(NULL, "|\n");
                char *year = strtok(NULL, "|\n");
                char *status = strtok(NULL, "|\n");
                if (cur == id) {
                    found = 1;
                    if (borrow) {
                        if (status && strcmp(status, "available") == 0) {
                            fprintf(tmp, "%lld|%s|%s|%s|borrowed\n", cur, title ? title : "", author ? author : "", year ? year : "");
                            updated = 1;
                        } else {
                            fprintf(tmp, "%lld|%s|%s|%s|%s\n", cur, title ? title : "", author ? author : "", year ? year : "", status ? status : "");
                        }
                    } else {
                        if (status && strcmp(status, "borrowed") == 0) {
                            fprintf(tmp, "%lld|%s|%s|%s|available\n", cur, title ? title : "", author ? author : "", year ? year : "");
                            updated = 1;
                        } else {
                            fprintf(tmp, "%lld|%s|%s|%s|%s\n", cur, title ? title : "", author ? author : "", year ? year : "", status ? status : "");
                        }
                    }
                } else {
                    fputs(line, tmp);
                }
            }
            fclose(fp); fclose(tmp);
            if (!found) {
                remove("tmp_books.txt");
                printf("Book ID not found.\n");
            } else {
                remove("books.txt");
                rename("tmp_books.txt", "books.txt");
                if (updated) printf("%s successful.\n", borrow ? "Borrow" : "Return");
                else printf("%s could not be completed (book maybe already in that state).\n", borrow ? "Borrow" : "Return");
            }
            waitForEnter();

        } else if (choice == 6) {
            printf("Enter Book ID to delete: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL) continue;
            long long id = strtoll(buf, NULL, 10);
            FILE *fp = fopen("books.txt", "r");
            if (!fp) { printf("No books file.\n"); waitForEnter(); continue; }
            FILE *tmp = fopen("tmp_books.txt", "w");
            if (!tmp) { fclose(fp); printf("Error.\n"); continue; }
            char line[2048]; int found = 0;
            while (fgets(line, sizeof(line), fp)) {
                char copy[2048]; strncpy(copy, line, sizeof(copy));
                char *p = strtok(copy, "|\n");
                if (!p) continue;
                long long cur = strtoll(p, NULL, 10);
                if (cur == id) { found = 1; continue; }
                fputs(line, tmp);
            }
            fclose(fp); fclose(tmp);
            if (found) {
                remove("books.txt");
                rename("tmp_books.txt", "books.txt");
                printf("Book deleted.\n");
            } else {
                remove("tmp_books.txt");
                printf("Book ID not found.\n");
            }
            waitForEnter();

        } else if (choice == 7) {
            return;
        } else {
            printf("Invalid choice.\n");
        }
    }
}
