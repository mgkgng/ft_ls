#include "ft_ls.h"

t_ls args;

typedef struct s_file {
    char *file;
    char *path;
    struct stat stat;
} t_file;

typedef struct s_dir {
    char *path;
    int max_len_size;
    int max_len_links;
    int total;
    t_list *files;
    t_list *subdirs;
} t_dir;


void print_ls_err(char *path) {
    ft_putstr_fd("ft_ls: ", 2);
    ft_putstr_fd(path, 2);
    ft_putstr_fd(": ", 2);
    perror(0);
}

void print_mode(mode_t mode) {
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

void print_owner(uid_t uid) {
    struct passwd *pwd = getpwuid(uid);
    if (!pwd) {
        perror("ft_ls");
        return ;
    }
    ft_putstr(pwd->pw_name);
    ft_putstr(" ");
}

void print_group(gid_t gid) {
    struct group *grp = getgrgid(gid);
    if (!grp) {
        perror("ft_ls");
        return ;
    }
    ft_putstr(grp->gr_name);
    ft_putstr(" ");
}

void print_time(time_t time) {
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

void print_nlinks(nlink_t nlinks) {
    ft_putnbr(nlinks);
    ft_putstr(" ");
}

void print_size(off_t size) {
    char str[10];
    char *size_str = ft_lltoa(size);
    int i = 0;
    while (i < 9 - (int) ft_strlen(size_str))
        str[i++] = ' ';
    ft_strcpy(str + i, size_str);
    ft_putstr(str);
    ft_putstr(" ");
    free(size_str);
}

int compare_files(void *a, void *b, int flags) {
    t_file *file_a = (t_file *) a;
    t_file *file_b = (t_file *) b;
    // if (flags & FLAG_S)
    //     return (flags & FLAG_REV) ? file_b->stat.st_size - file_a->stat.st_size : file_a->stat.st_size - file_b->stat.st_size;
    if (flags & FLAG_T)
        return (flags & FLAG_REV) ? file_b->stat.st_mtime - file_a->stat.st_mtime : file_a->stat.st_mtime - file_b->stat.st_mtime;
    return (flags & FLAG_REV) ? ft_strcmp(file_b->file, file_a->file) : ft_strcmp(file_a->file, file_b->file);
}

t_dir *get_file_list(char *path) {
    t_list *files = NULL;
    t_list *subdirs = NULL;
    int max_len_size = 0;
    int max_len_links = 0;
    int total = 0;

    DIR *dir = opendir(path);
    if (!dir) {
        print_ls_err(path);
        return (NULL);
    }
    struct dirent *entry;
    while ((entry = readdir(dir))) {
        char *filename = entry->d_name;
        if (filename[0] == '.' && !(args.options & FLAG_A))
            continue ;

        t_file *file = malloc(sizeof(t_file));
        file->file = ft_strdup(filename);
        char *file_path = ft_strjoin(path, "/");
        file_path = ft_strjoin(file_path, filename);
        file->path = file_path;

        struct stat statbuf;
        if (lstat(file_path, &statbuf) == -1) {
            print_ls_err(path);
            continue ;
        }
        if (S_ISDIR(statbuf.st_mode))
            ft_lstadd_back(&subdirs, ft_lstnew(ft_strdup(filename)));

        file->stat = statbuf;
        max_len_size = MAX(max_len_size, statbuf.st_size);
        max_len_links = MAX(max_len_links, statbuf.st_nlink);
        total += statbuf.st_blocks;
        ft_lstadd_back(&files, ft_lstnew(file));
    }
    closedir(dir);
    ft_lstsort(&files, (args.options & FLAG_REV), compare_files);

    t_dir *res = malloc(sizeof(t_dir));
    res->path = path;
    res->files = files;
    // res->subdirs = subdirs;
    res->max_len_links = max_len_links;
    res->max_len_size = max_len_size;
    res->total = total;
    return (res);
}

void ft_ls(char *path, char *file) {
    if (file) {
        path = ft_strjoin(path, "/");
        path = ft_strjoin(path, file);
        ft_putstr(path);
        ft_putendl(":"); 
    }

    struct stat statbuf;
    if (lstat(path, &statbuf) == -1) {
        print_ls_err(path);
        return ;
    }

    if (!S_ISDIR(statbuf.st_mode)) {

        ft_putendl((file) ? file : path);
        // Should get information (if flag)
        return ;
    }

    t_dir *dir = get_file_list(path);
    if (!dir)
        return ;
    t_list *curr = dir->files;
    while (curr) {
        if (args.options & FLAG_L) {
            print_mode(FILE(curr, stat.st_mode));
            print_nlinks(FILE(curr, stat.st_nlink));
            print_owner(FILE(curr, stat.st_uid));
            print_group(FILE(curr, stat.st_gid));
            print_size(FILE(curr, stat.st_size));
            print_time(FILE(curr, stat.st_mtime));
        }
        ft_putendl(FILE(curr, file));
        curr = curr->next;
    }

    
    if (dir->subdirs) {
        curr = dir->subdirs;
        while (curr) {
            ft_putendl("");
            ft_ls(path, FILE(curr, file));
            curr = curr->next;
        }
    }
}

int main(int ac, char **av) {
    args = parse(ac, av);
    
    t_list *begin = args.files;
    while (begin) {
        ft_ls(begin->content, false);
        begin = begin->next;
    }
    return 0;
}
