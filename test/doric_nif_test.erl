-module(doric_nif_test).

-include_lib("eunit/include/eunit.hrl").


doric_nif_test() ->
    ?assertEqual(ok, doric_nif:run()),
    ok.
