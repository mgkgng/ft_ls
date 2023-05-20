#include "ft_ls.h"

t_ls args;

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
    ft_putstr(time_str);
}

void print_size(off_t size) {
    ft_putnbr(size);
}

void ft_ls(char *path, char *file) {
    if (file) {
        path = ft_strjoin(path, "/");
        path = ft_strjoin(path, file);
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

    if (file) {
        ft_putstr(path);
        ft_putendl(":");   
    }

    DIR *dir = opendir(path);
    if (!dir) {
        perror("ft_ls");
        return ;
    }
    struct dirent *entry;
    t_list *files = NULL;
    while ((entry = readdir(dir))) {
        if (entry->d_name[0] == '.' && !(args.options & FLAG_A))
            continue ;
        ft_lstadd_back(&files, ft_lstnew(ft_strdup(entry->d_name)));
    }
    closedir(dir);
    ft_lstsort(&files, (args.options & FLAG_REV));
    while (files) {
        ft_putendl(files->content);

        if (args.options & FLAG_RECUR) {
            ft_ls(path, files->content);
        }

        else if (args.options & FLAG_L) {
            struct stat file_statbuf;
            char *file_path = ft_strjoin(path, "/");
            file_path = ft_strjoin(file_path, files->content);

            if (lstat(file_path, &file_statbuf) == -1) {
                print_ls_err(path);
                return ;
            }

            print_mode(file_statbuf.st_mode);
            // print number of links
            print_owner(file_statbuf.st_uid);
            print_group(file_statbuf.st_gid);
            print_size(file_statbuf.st_size);
            print_time(file_statbuf.st_mtime);
            ft_putendl(files->content);
        }
        
        files = files->next;
    }

    // if recursive mode, call ft_ls on this directory
    // if (args.options & FLAG_RECUR) {
        // ft_ls(path, args);
    // }
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
