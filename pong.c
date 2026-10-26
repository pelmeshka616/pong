#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_LEN 3
#define PADDLE1_X 3
#define PADDLE2_X 76
#define WIN_SCORE 21
#define SCORE_INDENT 34

void render_frame(int paddle1_y, int paddle2_y, int ball_x, int ball_y, int score1, int score2);
char cell_char(int row, int col, int paddle1_y, int paddle2_y, int ball_x, int ball_y);

int upper(int ch);
int read_first_command(void);
int read_second_command(void);
void consume_rest_of_line(void);

int move_paddle(int y, int delta);
int update_paddle1(int cmd, int y);
int update_paddle2(int cmd, int y);

int next_ball_x(int x, int dx);
int next_ball_y(int y, int dy);
int bounce_dy(int y, int dy);
int bounce_dx(int x, int y, int dx, int paddle1_y, int paddle2_y);
int reset_ball_x(void);
int reset_ball_y(void);

int is_goal(int x);
int update_score(int score, int should_increment);
int is_goal(int x);

int update_score(int score, int should_increment);
int check_winner(int score1, int score2);

void print_winner(int winner);

int main(void) {
    int paddle1_y = HEIGHT / 2 - PADDLE_LEN / 2;
    int paddle2_y = HEIGHT / 2 - PADDLE_LEN / 2;
    int ball_x = reset_ball_x();
    int ball_y = reset_ball_y();
    int ball_dx = 1;
    int ball_dy = 1;
    int score1 = 0;
    int score2 = 0;
    int winner = 0;
    render_frame(paddle1_y, paddle2_y, ball_x, ball_y, score1, score2);
    while (winner == 0) {
        int cmd1 = read_first_command();
        int cmd2 = read_second_command();
        consume_rest_of_line();
        paddle1_y = update_paddle1(cmd1, paddle1_y);
        paddle1_y = update_paddle1(cmd2, paddle1_y);
        paddle2_y = update_paddle2(cmd1, paddle2_y);
        paddle2_y = update_paddle2(cmd2, paddle2_y);

        ball_dy = bounce_dy(ball_y, ball_dy);
        ball_dx = bounce_dx(ball_x, ball_y, ball_dx, paddle1_y, paddle2_y);
        ball_x = next_ball_x(ball_x, ball_dx);
        ball_y = next_ball_y(ball_y, ball_dy);
        int goal = is_goal(ball_x);
        if (goal != 0) {
            score1 = update_score(score1, goal == 1);
            score2 = update_score(score2, goal == 2);
            ball_x = reset_ball_x();
            ball_y = reset_ball_y();
        }
        render_frame(paddle1_y, paddle2_y, ball_x, ball_y, score1, score2);
        winner = check_winner(score1, score2);
    }
    print_winner(winner);
    return 0;
}

void render_frame(int paddle1_y, int paddle2_y, int ball_x, int ball_y, int score1, int score2) {
    for (int i = 0; i < SCORE_INDENT; i++) {
        printf(" ");
    }
    printf("SCORE: %d : %d\n", score1, score2);
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", cell_char(i, j, paddle1_y, paddle2_y, ball_x, ball_y));
        }
        printf("\n");
    }
}

char cell_char(int row, int col, int paddle1_y, int paddle2_y, int ball_x, int ball_y) {
    char symbol;
    if (row == ball_y && col == ball_x) {
        symbol = 'o';
    } else if (col == PADDLE1_X && paddle1_y <= row && row < paddle1_y + PADDLE_LEN) {
        symbol = '#';
    } else if (col == PADDLE2_X && paddle2_y <= row && row < paddle2_y + PADDLE_LEN) {
        symbol = '#';
    } else if (row == 0 || row == HEIGHT - 1) {
        symbol = '-';
    } else if (col == 0 || col == WIDTH - 1) {
        symbol = '|';
    } else
        symbol = ' ';

    return symbol;
}

int upper(int ch) {
    if (ch >= 'a' && ch <= 'z') {
        ch = ch - 'a' + 'A';
    }
    return ch;
}

int read_first_command(void) {
    printf("Введите действие первого игрока: ");
    int ch = upper(getchar());
    int result;

    if (ch == 'A' || ch == 'Z' || ch == 'K' || ch == 'M' || ch == ' ') {
        result = ch;
    } else {
        result = -1;
    }

    return result;
}

int read_second_command(void) {
    int sep = getchar();
    int result;

    if (sep == '\n') {
        printf("Введите действие второго игрока: ");
        int ch = upper(getchar());

        if (ch == 'A' || ch == 'Z' || ch == 'K' || ch == 'M' || ch == ' ') {
            result = ch;
        } else {
            result = -1;
        }
    } else {
        result = -1;
    }

    return result;
}

void consume_rest_of_line(void) {
    int ch = getchar();

    while (ch != '\n' && ch != EOF) {
        ch = getchar();
    }
}

int move_paddle(int y, int delta) {
    int new_y = y + delta;
    int result;

    if (new_y < 1) {
        result = 1;
    } else if (new_y > HEIGHT - 1 - PADDLE_LEN) {
        result = HEIGHT - 1 - PADDLE_LEN;
    } else {
        result = new_y;
    }

    return result;
}

int update_paddle1(int cmd, int y) {
    int result;

    if (cmd == 'A') {
        result = move_paddle(y, -1);
    } else if (cmd == 'Z') {
        result = move_paddle(y, 1);
    } else {
        result = y;
    }

    return result;
}

int update_paddle2(int cmd, int y) {
    int result;

    if (cmd == 'K') {
        result = move_paddle(y, -1);
    } else if (cmd == 'M') {
        result = move_paddle(y, 1);
    } else {
        result = y;
    }

    return result;
}

int next_ball_x(int x, int dx) { return x + dx; }

int next_ball_y(int y, int dy) { return y + dy; }

int bounce_dy(int y, int dy) {
    int ball_dy = 0;

    if (y == 1)
        ball_dy = 1;
    else if (y == HEIGHT - 2)
        ball_dy = -1;
    else
        ball_dy = dy;

    return ball_dy;
}

int bounce_dx(int x, int y, int dx, int paddle1_y, int paddle2_y) {
    int ball_dx = 0;
    if (x == PADDLE1_X + 1 && y >= paddle1_y && y < paddle1_y + PADDLE_LEN) {
        ball_dx = 1;
    } else if (x == PADDLE2_X - 1 && y >= paddle2_y && y < paddle2_y + PADDLE_LEN) {
        ball_dx = -1;
    } else {
        ball_dx = dx;
    }
    return ball_dx;
}

int reset_ball_x(void) { return WIDTH / 2; }
int reset_ball_y(void) { return HEIGHT / 2; }

int is_goal(int x) {
    int goal = 0;
    if (x == 1) {
        goal = 2;
    } else if (x == WIDTH - 1) {
        goal = 1;
    } else {
        goal = 0;
    }
    return goal;
}

int update_score(int score, int should_increment) {
    int new_score = score;

    if (should_increment) new_score += 1;

    return new_score;
}

int check_winner(int score1, int score2) {
    int winner = 0;
    if (score1 == WIN_SCORE) {
        winner = 1;
    }
    if (score2 == WIN_SCORE) {
        winner = 2;
    }
    return winner;
}

void print_winner(int winner) { printf("Player %d wins!\n", winner); }
