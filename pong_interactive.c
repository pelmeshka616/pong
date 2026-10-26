
#include <ncurses.h>
#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define PADDLE_LEN 3
#define PADDLE1_X 3
#define PADDLE2_X 76
#define WIN_SCORE 21
#define SCORE_INDENT 34
#define FRAME_DELAY 120

void render_frame(int paddle1_y, int paddle2_y, int ball_x, int ball_y, int score1, int score2);
char cell_char(int row, int col, int paddle1_y, int paddle2_y, int ball_x, int ball_y);
void consume_rest_of_line(void);
int move_paddle(int y, int delta);
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
void render_frame(int paddle1_y, int paddle2_y, int ball_x, int ball_y, int score1, int score2);
int read_input_p(void);
void print_winner(int winner);

int main(void) {
    initscr();
    curs_set(0);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    int paddle1_y = HEIGHT / 2 - PADDLE_LEN / 2, paddle2_y = HEIGHT / 2 - PADDLE_LEN / 2;
    int ball_x = WIDTH / 2, ball_y = HEIGHT / 2;
    int ball_dx = 1, ball_dy = 1;
    int score1 = 0, score2 = 0;
    int winner = 0;

    while (winner == 0) {
        int ch = read_input_p();

        if (ch == 'A' || ch == 'a') {
            paddle1_y = move_paddle(paddle1_y, -1);
        } else if (ch == 'Z' || ch == 'z')
            paddle1_y = move_paddle(paddle1_y, 1);

        if (ch == 'K' || ch == 'k') {
            paddle2_y = move_paddle(paddle2_y, -1);
        } else if (ch == 'M' || ch == 'm')
            paddle2_y = move_paddle(paddle2_y, 1);

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

        napms(FRAME_DELAY);
    }
    endwin();
    print_winner(winner);
    return 0;
}

void render_frame(int paddle1_y, int paddle2_y, int ball_x, int ball_y, int score1, int score2) {
    clear();

    mvprintw(26, SCORE_INDENT, "SCORE: %d : %d", score1, score2);

    for (int i = 0; i < HEIGHT; ++i) {
        mvaddch(i, 0, '|');
        mvaddch(i, WIDTH - 1, '|');
    }
    for (int j = 0; j < WIDTH; ++j) {
        mvaddch(0, j, '-');
        mvaddch(HEIGHT - 1, j, '-');
    }

    for (int i = 0; i < PADDLE_LEN; ++i) {
        mvaddch(paddle1_y + i, PADDLE1_X, '#');
        mvaddch(paddle2_y + i, PADDLE2_X, '#');
    }

    mvaddch(ball_y, ball_x, 'o');

    refresh();
}

int read_input_p(void) {
    int ch = getch();
    if (ch == ERR) return 0;
    return ch;
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
