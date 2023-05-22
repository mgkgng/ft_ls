#include "ft_ls.h"

int compare_files(void *a, void *b, int flags) {
    t_file *file_a = (t_file *) a;
    t_file *file_b = (t_file *) b;

    if (flags & FLAG_T) {
        int res = (flags & FLAG_REV) ? file_a->stat.st_mtime - file_b->stat.st_mtime : file_b->stat.st_mtime - file_a->stat.st_mtime;
        if (res != 0)
            return (res);
    }
    return (flags & FLAG_REV) ? ft_strcmp(file_b->file, file_a->file) : ft_strcmp(file_a->file, file_b->file);
}

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
        if ((args.options & FLAG_RECUR) && S_ISDIR(statbuf.st_mode))
            ft_lstadd_back(&subdirs, ft_lstnew(ft_strdup(filename)));

        file->stat = statbuf;
        max_len_size = MAX(max_len_size, ft_numlen(statbuf.st_size));
        max_len_links = MAX(max_len_links, ft_numlen(statbuf.st_nlink));
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

void ft_ls(char *path, char *file) {
    if (file) {
        path = ft_strjoin(path, "/", false);
        path = ft_strjoin(path, file, true);
        ft_putstr(path);
        ft_putendl(":"); 
    }

    struct stat statbuf;
    if (lstat(path, &statbuf) == -1) {
        print_ls_err(path);
        return ;
    }

    if (!S_ISDIR(statbuf.st_mode)) {
        if (args.options & FLAG_L)
            print_long_format(statbuf, 0 ,0);
        ft_putendl((file) ? file : path);
        // Should get information (if flag)
        return ;
    }

    t_dir *dir = get_dir_info(path);
    if (!dir)
        return ;
    if (args.options & FLAG_L) {
        ft_putstr("total ");
        ft_putnbr(dir->total);
        ft_putendl("");
    }

    t_list *curr = dir->files;
    while (curr) {
        if (args.options & FLAG_L)
            print_long_format(FILE(curr, stat), dir->max_len_links, dir->max_len_size);
        ft_putendl(FILE(curr, file));
        curr = curr->next;
    }
    if (dir->subdirs) {
        curr = dir->subdirs;
        while (curr) {
            ft_putendl("");
            ft_ls(path, curr->content);
            curr = curr->next;
        }
    }
    if (file)
        free(path);
    terminate(dir);
}