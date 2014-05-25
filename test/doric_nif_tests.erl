-module(doric_nif_tests).


-compile(export_all).

-include_lib("eunit/include/eunit.hrl").

doric_nif_test() ->
    %%?assertEqual(ok, doric_nif:run()),

    doric_nif:new_table(spam, {}),
    doric_nif:add_row(spam, {}),
    ?debugVal(doric_nif:min(spam, col1)),
    ?debugVal(doric_nif:max(spam, col1)),
    doric_nif:drop_table(spam),
    ok.
