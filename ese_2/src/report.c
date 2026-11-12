/* Release report code: you do not need to read or modify this file :)
 *
 * The confirmation code is encrypted: it can only be decrypted with a key
 * computed from the correct result of the exercise.
 * The revealed code is bound to the current user and machine.
 */
#include "report.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>

#define FNV_START 2166136261u

static unsigned int mix(unsigned int h, unsigned int v) {
    for (int i = 0; i < 4; i++) {
        h ^= (v >> (8 * i)) & 0xff;
        h *= 16777619u;
    }
    return h;
}

static unsigned int next_byte(unsigned int *s) {
    *s ^= *s << 13;
    *s ^= *s >> 17;
    *s ^= *s << 5;
    return *s & 0xff;
}

/* Decrypts 'len' bytes of 'enc' into 'out' and returns its checksum */
static unsigned int decrypt(unsigned int key, const unsigned char *enc, int len, char *out) {
    unsigned int s = key ? key : 1;
    unsigned int h = FNV_START;
    for (int i = 0; i < len; i++) {
        out[i] = enc[i] ^ next_byte(&s);
        h = mix(h, (unsigned char)out[i]);
    }
    out[len] = '\0';
    return h;
}

/* Appends to 'word' a code bound to the current user and machine */
static void bind_to_user(char *out, size_t size, const char *word) {
    const char *user = getenv("USER");
    char host[256] = "";
    char id[600];

    if (user == NULL || user[0] == '\0')
        user = "unknown";
    if (gethostname(host, sizeof(host)) != 0 || host[0] == '\0')
        strcpy(host, "unknown");
    host[sizeof(host) - 1] = '\0';

    snprintf(id, sizeof(id), "%s:%s@%s", word, user, host);
    unsigned int h = FNV_START;
    for (const char *c = id; *c != '\0'; c++) {
        h ^= (unsigned char)*c;
        h *= 16777619u;
    }
    snprintf(out, size, "%s-%04X", word, h & 0xffff);
}

static void reveal(int number, unsigned int key, const unsigned char *enc, int len, unsigned int check) {
    char word[32];
    char bound[48];
    if (decrypt(key, enc, len, word) != check)
        return;
    bind_to_user(bound, sizeof(bound), word);
    printf("\n*** Release report, code #%d of 5: \"%s\" ***\n", number, bound);
}

#include "page.h"

/* Code #2: the reply segment holds exactly the requested page, it was
 * created by this process, and only this process is still attached to it */
void report_check(int replyShmid, int pageNo, const char *page) {
    static const unsigned char enc[] = {0x19, 0x84, 0xe9, 0xfd, 0x41, 0xc9};
    static char expected[PAGE_SIZE];
    struct shmid_ds ds;
    int ok = page != NULL && pageNo > 0;
    if (ok) {
        loadPage(pageNo, expected);
        if (strncmp(page, expected, PAGE_SIZE) != 0)
            ok = 0;
    }
    if (ok && (shmctl(replyShmid, IPC_STAT, &ds) == -1 || ds.shm_nattch != 1 ||
               ds.shm_cpid != getpid() || ds.shm_segsz < PAGE_SIZE))
        ok = 0;
    unsigned int key = mix(mix(FNV_START, 0x62), ok);
    reveal(2, key, enc, sizeof(enc), 0xa5cf1e56u);
}
