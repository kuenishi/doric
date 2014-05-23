#include "doric_nif.h"
#include <stdio.h>

static ErlNifFunc nif_funcs[] =
{
  /**
    {"close", 1, eleveldb_close},
    {"iterator_close", 1, eleveldb_iterator_close},
    {"status", 2, eleveldb_status},
    {"destroy", 2, eleveldb_destroy},
    {"repair", 2, eleveldb_repair},
    {"is_empty", 1, eleveldb_is_empty},

    {"async_open", 3, eleveldb::async_open},
    {"async_write", 4, eleveldb::async_write},
    {"async_get", 4, eleveldb::async_get},

    {"async_iterator", 3, eleveldb::async_iterator},
    {"async_iterator", 4, eleveldb::async_iterator},

    {"async_iterator_move", 3, eleveldb::async_iterator_move}
  **/
  {"run", 0, run}
};

ERL_NIF_TERM run(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[])
{
  printf("hello\n");
  return enif_make_atom(env, "ok");
}

static int on_load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info)
{
  return 0;
}

static int upgrade(ErlNifEnv* env, void** priv_data, void** old_priv_data, ERL_NIF_TERM load_info)
{
  return 0;
}

static void on_unload(ErlNifEnv *env, void *priv_data)
{
}

ERL_NIF_INIT(doric_nif, nif_funcs, &on_load, NULL, &upgrade, &on_unload);
