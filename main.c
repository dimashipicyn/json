#include "map.h"
#include "libft.h"
#include "btree.h"
#include "json.h"

#include <stdio.h>
#include <assert.h>

void bar(void* s)
{
    printf("val: %s\n", s);
}

void foo()
{
    t_btree* root = NULL;

    btree_insert(&root, "5", ft_strcmp);
    btree_insert(&root, "3", ft_strcmp);
    btree_insert(&root, "7", ft_strcmp);
    btree_insert(&root, "2", ft_strcmp);
    btree_insert(&root, "4", ft_strcmp);
    btree_insert(&root, "6", ft_strcmp);
    btree_insert(&root, "8", ft_strcmp);
    btree_insert(&root, "9", ft_strcmp);

    btree_apply_infix(root, bar);
    btree_clear(&root);
    
}
int ident = 0;
void print_ident(int ident)
{
    while (ident-- > 0) {
        printf("\t");
    }
}

void print_json(void* obj);
void print_json_map(void* val)
{
    struct kv {
        char* key;
        void* val;
    };
    ident++;
    print_ident(ident);
    printf("%s: ", ((struct kv*)val)->key);
    print_json(((struct kv*)val)->val);
    ident--;
}
void print_json(void* obj)
{
    if (obj == NULL) {
        return;
    }
    json_unknown* tmp = (json_unknown*)obj;
    switch (tmp->type)
    {
    case JSON_OBJECT:
        printf("{\n");
        btree_apply_infix(tmp->value, print_json_map);
        print_ident(ident);
        printf("}\n");
        break;
    case JSON_STRING:
        printf("%s\n", tmp->value);
        break;
    default:
        break;
    }
}

void test_json()
{
    json_object* obj = new_json_object();
    json_object* obj1 = new_json_object();
    json_object* obj2 = new_json_object();
    //json_array* arr = new_json_array();
    json_string* str = new_json_string("petuh");
    json_string* str1 = new_json_string("petuh1");
    json_string* str2 = new_json_string("petuh2");
    json_string* str3 = new_json_string("petuh3");
    json_string* str4 = new_json_string("petuh4");
    //json_object_insert(obj, "hello", arr);
    json_object_insert(obj, "cmo", str);
    json_object_insert(obj, "cmo1", str1);
    json_object_insert(obj, "cmo2", str2);

    json_object_insert(obj1, "cmo3", str3);
    json_object_insert(obj1, "cmo5", str4);
    json_object_insert(obj, "cmo4", obj1);


    json_object_insert(obj, "valera", obj2);
    
    //foo();
    print_json(obj);
    delete_json(obj);
}


int main(int ac, char** av) {
    char* text = "{\"hello world\": {\"petuh\":\"cmo\"}, \"test\":\"keklol\"}";
    json_unknown* js;
    int err = json_parse(&js, text);
    if (err != 0) {
        printf("error\n");
    }
    json_unknown* u = json_object_find(js, "test");
    if (u)
        printf("value type: %d, test: %s\n", u->type, u->value);
    print_json(js);
    //test_json();
    //foo();
    //printf("\n");
    //while (1)
    //{
    //    ;
    //}
}
