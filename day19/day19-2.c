#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/dynstring.h"
#include "../utils/dynarray.h"

typedef struct SubRule {
    Dynarr references;
} SubRule;

typedef struct Rule {
    int id;
    enum Type {
        REFS,
        CHAR
    } type;
    union Value {
        Dynarr subrules;
        char c;
    } value;
} Rule;

void remove_space(char **str) {
    int n = -1;
    sscanf(*str, "%*[ ]%n", &n);
    if (n != -1) {
        *str += n;
    }
}

Rule parse_rule(String line) {
    char *rest = line.data;
    int n;
    int index;
    sscanf(rest, "%d:%n", &index, &n);
    rest += n;
    Rule rule;
    rule.id = index;
    if (rest[1] == '"') {
        rule.type = CHAR;
        rule.value.c = rest[2];
        return rule;
    } else {
        rule.type = REFS;
        rule.value.subrules = Dynarr_create(SubRule);
        while (*rest != '\0') {
            rest++;
            SubRule subRule;
            subRule.references = Dynarr_create(int);
            int index;
            int n;
            while (sscanf(rest, "%d%n", &index, &n) == 1) {
                subRule.references.insert(&subRule.references, &index);
                rest += n;
            }
            remove_space(&rest);
            rule.value.subrules.insert(&rule.value.subrules, &subRule);
        }
        return rule;
    }
}

int rule_cmp(const void *a, const void *b) {
    return ((Rule *)a)->id - ((Rule *)b)->id;
}

Dynarr fits_rule(Rule rule, char *str, Dynarr rules);

Rule get_rule(SubRule subRule, int index, Dynarr rules) {
    int id = *(int *)subRule.references.get(&subRule.references, index);
    return *(Rule *)rules.get(&rules, id);
}

Dynarr match_subrule(SubRule subRule, char *str, Dynarr rules) {
    Dynarr matches = fits_rule(get_rule(subRule, 0, rules), str, rules);
    for (int i = 1; i < subRule.references.len && matches.len > 0; i++) {
        Dynarr newMatches = Dynarr_create(char *);
        for (int j = 0; j < matches.len; j++) {
            Dynarr res = fits_rule(get_rule(subRule, i, rules),
                    *(char **)matches.get(&matches, j),
                    rules);
            for (int k = 0; k < res.len; k++) {
                char *r = *(char **)res.get(&res, k);
                newMatches.insert(&newMatches, &r);
            }
            res.free(&res);
        }
        matches.free(&matches);
        matches = newMatches;
    }
    return matches;
}

Dynarr fits_rule(Rule rule, char *str, Dynarr rules) {
    Dynarr results = Dynarr_create(char *);
    if (rule.type == CHAR) {
        if (*str == rule.value.c) {
            str++;
            results.insert(&results, &str);
        }
        return results;
    } else {
        for (int i = 0; i < rule.value.subrules.len; i++) {
            SubRule *subRule = rule.value.subrules.get(&rule.value.subrules, i);
            Dynarr matches = match_subrule(*subRule, str, rules);
            for (int j = 0; j < matches.len; j++) {
                char *r = *(char **)matches.get(&matches, j);
                results.insert(&results, &r);
            }
            matches.free(&matches);
        }
        return results;
    }
}

void subrule_free(void *a) {
    SubRule *subRule = (SubRule *)a;
    subRule->references.free(&subRule->references);
}

void rule_free(void *a) {
    Rule *rule = (Rule *)a;
    if (rule->type == REFS) {
        rule->value.subrules.execute(&rule->value.subrules, subrule_free);
        rule->value.subrules.free(&rule->value.subrules);
    }
}

int main() {
    Dynarr rules = Dynarr_create(Rule);
    String line;
    while ((line = String_readline()).len > 0) {
        Rule rule = parse_rule(line);
        rules.insert(&rules, &rule);

        String_free(&line);
    }
    String_free(&line);
    rules.sort(&rules, rule_cmp);

    Rule *rule8 = (Rule *)rules.get(&rules, 8);
    SubRule sr42_8;
    sr42_8.references = Dynarr_create(int);
    int i = 42;
    sr42_8.references.insert(&sr42_8.references, &i);
    i = 8;
    sr42_8.references.insert(&sr42_8.references, &i);
    rule8->value.subrules.insert(&rule8->value.subrules, &sr42_8);
    Rule *rule11 = (Rule *)rules.get(&rules, 11);
    SubRule sr42_11_31;
    sr42_11_31.references = Dynarr_create(int);
    i = 42;
    sr42_11_31.references.insert(&sr42_11_31.references, &i);
    i = 11;
    sr42_11_31.references.insert(&sr42_11_31.references, &i);
    i = 31;
    sr42_11_31.references.insert(&sr42_11_31.references, &i);
    rule11->value.subrules.insert(&rule11->value.subrules, &sr42_11_31);

    int count = 0;
    while ((line = String_readline()).len > 0) {
        Dynarr matches = fits_rule(*(Rule *)rules.get(&rules, 0), line.data, rules);
        for (int i = 0; i < matches.len; i++) {
            char *r = *(char **)matches.get(&matches, i);
            if (*r == '\0') {
                count++;
            }
        }
        matches.free(&matches);

        String_free(&line);
    }
    String_free(&line);
    printf("%d\n", count);

    rules.execute(&rules, rule_free);
    rules.free(&rules);

    return 0;
}
