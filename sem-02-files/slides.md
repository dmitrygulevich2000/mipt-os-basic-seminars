---
marp: true
paginate: true
---

<style>
img[alt~="center"] {
  display: block;
  margin: 0 auto;
}
.columns {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
  gap: 1rem;
}
</style>

# Файловые системы

---

# `open(2)`

- `int open(const char *pathname, int flags, mode_t mode);`
- `flags` - бит-сет опций для открытия файла
- `mode` - права доступа, если файл создаётся
- возвращает `fd` - *файловый дескриптор* - "ссылку" на открытый файл
- `fd` должен быть закрыт сисколлом `close`

---

# `flags`

* access modes: `O_RDONLY`, `O_WRONLY`, `O_RDWR`
* status flags:
* `O_APPEND`, `O_TRUNC` 
* `O_CREAT`, `O_EXCL`
* ...

---

# `mode`

<!-- "all" html should be enabled -->
<div data-marpit-fragment>

- 3 права на доступ (3 бита) - **r**ead, **w**rite, e**x**ecute
- для 3 уровней - **u**ser, **g**roup, **o**thers

</div>

<div data-marpit-fragment>

- то, что показывает `ls -l`
- удобно обозначить восьмеричным числом из 3 цифр
  - `0764` ~ `rwxrw-r--`

</div>

---

# `read(2)`

* `ssize_t read(int fd, void *buf, size_t count);`
* читает не более `count` байт из файла, указанного файловым дескриптором `fd`, и записывает данные в буфер по адресу `buf`
* возвращает:
  * число прочитанных байт (может быть меньше, чем `count`)
  * 0, если достигнут конец файла

---

# `write(2)`

* `ssize_t write(int fd, const void *buf, size_t count);`
* записывает в файл, указанный `fd`, `count` байт из буфера `buf`
* возвращает число записанных байт (может быть меньше, чем `count`)

---

# Ошибки

<div data-marpit-fragment>

- результат вызова -1 - индикатор ошибки
- код ошибки сохраняется в ~~глобальной переменной~~ `errno`
- в заголовочных файлах объявлены константы для кодов

</div>

<div data-marpit-fragment>

- вывод ошибок: `perror(3)`, `strerror(3)`, `err(3)`
- консольная команда `errno(1)`
- подробное описание ошибок - в манах сисколлов

</div>

---

# `stat(2)`

`int stat(const char *pathname, struct stat *statbuf);`

```
struct stat {
    ...
    mode_t    st_mode;        /* File type and mode. See inode(7) */
    uid_t     st_uid;         /* User ID of owner */
    gid_t     st_gid;         /* Group ID of owner */
    off_t     st_size;        /* Total size, in bytes */
    ...

    struct timespec st_mtim;  /* Time of last modification */
    ...
};
```

---

# `stat(2)`

- `st.st_mode & 0777` - права доступа
- `st.st_mode & S_IFMT` - тип файла
  - `st.st_mode & S_IFMT == S_IFREG`
  - эквивалентно `S_ISREG(m)`
- а помимо regular file есть: directory, symbolic link, block device, character device, FIFO, socket

---

# Ссылки

- бывают *жёсткие* и *символические*
- `ln` поможет создать
- а `ls -li` всё покажет

```
struct stat {
    ...
    dev_t     st_dev;         /* ID of device containing file */
    ino_t     st_ino;         /* Inode number */
    nlink_t   st_nlink;       /* Number of hard links */
    ...
};
```

---

# GG, WP
