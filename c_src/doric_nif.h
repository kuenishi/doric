#pragma once

#include <erl_nif.h>

extern "C" {

ERL_NIF_TERM run(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[]);
ERL_NIF_TERM new_table(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[]);
ERL_NIF_TERM drop_table(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[]);
ERL_NIF_TERM add_row(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[]);
ERL_NIF_TERM min(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[]);
ERL_NIF_TERM max(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[]);

}
