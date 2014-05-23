%%% @copyright (C) 2014, UENISHI Kota
%%% @doc
%%%
%%% @end

-module(doric_nif).
-on_load(init/0).

-export([run/0]).

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
