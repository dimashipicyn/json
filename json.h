#ifndef JSON_H
#define JSON_H

typedef struct s_btree t_btree;
typedef struct s_list t_list;

typedef enum type {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    UNDEFINED
} t_type;

typedef struct {
    t_type type;
    void* value;
} json_unknown;

typedef struct {
    t_type type;
    t_btree *value;
} json_object;

typedef struct {
    t_type type;
    t_list *value;
} json_array;

typedef struct {
    t_type type;
    char *value;
} json_string;


json_object*    new_json_object();
void            delete_json_object(json_object* obj);
void            json_object_insert(json_object* self, const char* key, void* val);

json_array*     new_json_array();
void            delete_json_array(json_array* obj);

json_string*    new_json_string(const char* str);
void            delete_json_string(json_string* obj);

void            delete_json(void* obj);

int             json_parse(void** obj, const char* text);

#endif