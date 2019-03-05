#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../include/list.h"

#include "../private/common.h"

static uint16_t values[256];

static void list_mergesort(struct list_head *head)
{
    struct list_head list_left, list_right;

    if (list_empty(head) || list_is_singular(head))
        return;

    INIT_LIST_HEAD(&list_left);
    INIT_LIST_HEAD(&list_right);

    struct list_head *slow = head, *fast = head->next;
    while (fast != head) {
        fast = fast->next;
        if (fast != head) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    list_cut_position(&list_left, head, slow);
    list_splice_init(head, &list_right);

    list_mergesort(&list_left);
    list_mergesort(&list_right);

    while (1) {
        if (list_empty(&list_left)) {
            list_splice_tail(&list_right, head);
            break;
        }

        if (list_empty(&list_right)) {
            list_splice_tail(&list_left, head);
            break;
        }

        if (cmpint(&list_first_entry(&list_left, struct listitem, list)->i,
                   &list_first_entry(&list_right, struct listitem, list)->i) <
            0) {
            list_move_tail((&list_left)->next, head);
        } else {
            list_move_tail((&list_right)->next, head);
        }
    }
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item, *is = NULL;
    struct timeval start, end;
    size_t i;
    FILE *f = fopen(NAME, "w");

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
    list_mergesort(&testlist);
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
