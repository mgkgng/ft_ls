#include "ft_ls.h"

static void print_mode(mode_t mode) {
    char mode_str[11];

    mode_str[0] = (S_ISDIR(mode)) ? 'd' : (S_ISLNK(mode)) ? 'l' : '-';
    mode_str[1] = (mode & S_IRUSR) ? 'r' : '-';
    mode_str[2] = (mode & S_IWUSR) ? 'w' : '-';
    mode_str[3] = (mode & S_IXUSR) ? 'x' : '-';
    mode_str[4] = (mode & S_IRGRP) ? 'r' : '-';
    mode_str[5] = (mode & S_IWGRP) ? 'w' : '-';
    mode_str[6] = (mode & S_IXGRP) ? 'x' : '-';
    mode_str[7] = (mode & S_IROTH) ? 'r' : '-';
    mode_str[8] = (mode & S_IWOTH) ? 'w' : '-';
    mode_str[9] = (mode & S_IXOTH) ? 'x' : '-';
    mode_str[10] = '\0';
    ft_putstr(mode_str);
    ft_putstr("  ");
}

static void print_owner(uid_t uid) {
    struct passwd *pwd = getpwuid(uid);
    if (!pwd) {
        perror("ft_ls");
        return ;
    }
    ft_putstr(pwd->pw_name);
    ft_putstr(" ");
}

// static void print_group(gid_t gid) {
//     struct group *grp = getgrgid(gid);
//     if (!grp) {
//         perror("ft_ls");
//         return ;
//     }
//     ft_putstr(grp->gr_name);
//     ft_putstr(" ");
// }

static void print_time(time_t time) {
    char *time_str = ctime(&time);
    time_str[ft_strlen(time_str) - 1] = '\0';

    char *start_of_month = time_str + 4;
    char *start_of_day = start_of_month + 4;
    char *start_of_time = start_of_day + 3;

    start_of_month[-1] = '\0';
    start_of_day[-1] = '\0';
    start_of_time[-1] = '\0';
    start_of_time[5] = '\0';

    ft_putstr(start_of_day);
    ft_putstr(" ");
    ft_putstr(start_of_month);
    ft_putstr(" ");
    ft_putstr(start_of_time);
    ft_putstr(" ");
}

static void print_nlinks(nlink_t nlinks, int max_len_links) {
    while (max_len_links-- > ft_nbrlen(nlinks))
        ft_putchar(' ');
    ft_putnbr(nlinks);
    ft_putchar(' ');
}

static void print_size(off_t size, int max_len_size) {
    while (max_len_size-- > ft_nbrlen(size))
        ft_putchar(' ');
    ft_putnbr(size);
    ft_putchar(' ');
}

static void print_long_format(struct stat statbuf, int max_len_links, int max_len_size) {
    print_mode(statbuf.st_mode);
    print_nlinks(statbuf.st_nlink, max_len_links);
    print_owner(statbuf.st_uid);
    // print_group(statbuf.st_gid);
    print_size(statbuf.st_size, max_len_size);
    print_time(statbuf.st_mtime);
}

void display(t_list *lst, int max_len_links, int max_len_size) {
    t_list *curr = lst;
    while (curr) {
        if (args.options & FLAG_L)
            print_long_format(FILE(curr, stat), max_len_links, max_len_size);
        ft_putendl(FILE(curr, file));
        curr = curr->next;
    }
}