#include "ft_ls.h"

t_dir *get_dir_info(char *path) {
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

        t_file *file = ft_calloc(1, sizeof(t_file));
        file->file = ft_strdup(filename);
        char *file_path = ft_strjoin(path, "/", false);
        file_path = ft_strjoin(file_path, filename, true);
        file->path = file_path;

        struct stat statbuf;
        if (lstat(file_path, &statbuf) == -1) {
            print_ls_err(path);
            continue ;
        } 
        if (args.options & FLAG_L && S_ISLNK(statbuf.st_mode)) {
            char ltarget[256]; // file name length limit (<256)

            ssize_t len = readlink(file->path, ltarget, sizeof(ltarget) - 1);
            if (len != -1) {
                ltarget[len] = '\0';
                file->file = ft_strjoin(file->file, " -> ", false);
                file->file = ft_strjoin(file->file, ltarget, true);
            } else {
                perror("readlink");
                continue ;
            }
        }
        if ((args.options & FLAG_RECUR) && S_ISDIR(statbuf.st_mode)) {
            t_file *subdir = ft_calloc(1, sizeof(t_file));
            subdir->file = ft_strdup(filename);
            subdir->stat = statbuf;
            ft_lstadd_back(&subdirs, ft_lstnew(subdir));
        }
        file->stat = statbuf;
        max_len_size = MAX(max_len_size, ft_nbrlen(statbuf.st_size));
        max_len_links = MAX(max_len_links, ft_nbrlen(statbuf.st_nlink));
        total += statbuf.st_blocks;
        ft_lstadd_back(&files, ft_lstnew(file));
    }
    closedir(dir);
    ft_lstsort(&files, args.options, compare_files);

    t_dir *res = ft_calloc(1, sizeof(t_dir));
    res->path = path;
    res->files = files;
    res->subdirs = subdirs;
    res->max_len_links = max_len_links;
    res->max_len_size = max_len_size;
    res->total = total;
    return (res);
}

void ft_ls_dirs(char *path, char *file) {
    if (file) {
        path = ft_strjoin(path, "/", false);
        path = ft_strjoin(path, file, true);
        ft_putstr(path);
        ft_putendl(":"); 
    }

    t_dir *dir = get_dir_info(path);
    if (!dir)
        return ;
    if (args.options & FLAG_L) {
        ft_putstr("total ");
        ft_putnbr(dir->total / 2);
        ft_putendl("");
    }
    display(dir->files, dir->max_len_links, dir->max_len_size);
    if (dir->subdirs) {
        if (args.options & FLAG_T) {
            ft_lstsort(&dir->subdirs, args.options, compare_files);
        }
        t_list *curr = dir->subdirs;
        while (curr) {
            ft_putendl("");
            ft_ls_dirs(path, FILE(curr, file));
            curr = curr->next;
        }
    }
    if (file)
        free(path);
    terminate(dir);
}

void ft_ls_files(t_list *files) {
    t_list *summary = NULL;
    t_list *curr = files;
    int max_len_links = 0;
    int max_len_size = 0;
    while (curr) {
        struct stat statbuf;
        if (stat(FILE(curr, file), &statbuf) == -1) {
            if (!(args.options & FLAG_L) || lstat(FILE(curr, file), &statbuf) == -1) {
                print_ls_err(FILE(curr, file));
                curr = curr->next;
                continue ;
            }
        }
        if (S_ISLNK(statbuf.st_mode)) {
            char ltarget[256]; // file name length limit (<256)

            ssize_t len = readlink(curr->content, ltarget, sizeof(ltarget) - 1);
            if (len != -1) {
                ltarget[len] = '\0';
                FILE(curr, file) = ft_strjoin(FILE(curr, file), " -> ", false);
                FILE(curr, file) = ft_strjoin(FILE(curr, file), ltarget, true);
            } else {
                perror("readlink");
                curr = curr->next;
                continue ;
            }
        }
        t_file *file = ft_calloc(1, sizeof(t_file));
        file->file = FILE(curr, file);
        file->stat = statbuf;
        max_len_size = MAX(max_len_size, ft_nbrlen(statbuf.st_size));
        max_len_links = MAX(max_len_links, ft_nbrlen(statbuf.st_nlink));
        ft_lstadd_back(&summary, ft_lstnew(file));
        curr = curr->next;
    }
    display(summary, max_len_links, max_len_size);
    while (summary) {
        t_list *next = summary->next; 
        free(summary->content);
        free(summary);
        summary = next;
    }
}