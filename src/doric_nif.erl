%%% @copyright (C) 2014, UENISHI Kota
%%% @doc
%%%
%%% @end

-module(doric_nif).
-on_load(init/0).

-export([run/0,
         new_table/2, drop_table/1,
         add_row/2, min/2, max/2]).

-spec init() -> ok | {error, any()}.
init() ->
    SoName = case code:priv_dir(?MODULE) of
                 {error, bad_name} ->
                     case code:which(?MODULE) of
                         Filename when is_list(Filename) ->
                             filename:join([filename:dirname(Filename),"../priv", "doric_drv"]);
                         _ ->
                             filename:join("../priv", "doric_drv")
                     end;
                 Dir ->
                     filename:join(Dir, "doric_drv")
             end,
    erlang:load_nif(SoName, application:get_all_env(doric)).

run() ->
    throw(notyet).

new_table(TableName, Schema) ->
    notyet.

drop_table(TableName) ->
    notyet.

add_row(TableName, Row) ->
    notyet.

min(TableName, Column) ->
    notyet.

max(TableName, Column) ->
    notyet.
