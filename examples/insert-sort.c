#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../include/list.h"

#include "../private/common.h"

static uint16_t values[N];

static void list_insert_sorted(struct listitem *entry, struct list_head *head)
{
    struct listitem *item = NULL;

    if (list_empty(head)) {
        list_add(&entry->list, head);
        return;
    }

    list_for_each_entry (item, head, list) {
        if (cmpint(&entry->i, &item->i) < 0) {
            list_add_tail(&entry->list, &item->list);
            return;
        }
    }

    list_add_tail(&entry->list, head);
}

static void list_insertsort(struct list_head *head)
{
    struct list_head list_unsorted;
    struct listitem *item = NULL, *is = NULL;

    INIT_LIST_HEAD(&list_unsorted);
    list_splice_init(head, &list_unsorted);

    list_for_each_entry_safe (item, is, &list_unsorted, list) {
        list_del(&item->list);
        list_insert_sorted(item, head);
    }
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item = NULL, *is = NULL;
    struct timeval start, end;
    size_t i;
    FILE *f = fopen(NAME, "a+");

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));

    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));
    qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);
    gettimeofday(&start, NULL);
    list_insertsort(&testlist);
    gettimeofday(&end, NULL);
    fprintf(f, "%ld %lf\n", ARRAY_SIZE(values),
            timeval_diff(NULL, &end, &start));
    fclose(f);

    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == values[i]);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == ARRAY_SIZE(values));
    assert(list_empty(&testlist));

    return 0;
}
