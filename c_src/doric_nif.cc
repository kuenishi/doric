#include "doric_nif.h"
#include <stdio.h>
#include <iostream>


#include "supersonic/supersonic.h"

using std::cout;
using std::endl;

// Using-declarations for Supersonic names, pre-listed to make the test code
// more readable.
using supersonic::BoundExpressionTree;
using supersonic::Expression;
using supersonic::Plus;
using supersonic::AttributeAt;
using supersonic::TupleSchema;
using supersonic::Attribute;
using supersonic::DOUBLE;
using supersonic::INT32;
using supersonic::STRING;
using supersonic::NOT_NULLABLE;
using supersonic::FailureOrOwned;
using supersonic::HeapBufferAllocator;
using supersonic::View;
using supersonic::EvaluationResult;
using supersonic::AggregationSpecification;
using supersonic::SUM;
using supersonic::Operation;
using supersonic::ProjectNamedAttribute;
using supersonic::SingleSourceProjector;
using supersonic::GroupAggregate;
using supersonic::ScanView;
using supersonic::ResultView;
using supersonic::SucceedOrDie;
using supersonic::GroupAggregateOptions;
using supersonic::Cursor;

const int32* AddColumns(int32* a,
                        int32* b,
                        size_t row_count,
                        BoundExpressionTree* bound_tree) {
  // We have to redefine the input schema - we could also reuse the one defined
  // in PrepareBoundExpression.
  TupleSchema schema;
  schema.add_attribute(Attribute("a", INT32, NOT_NULLABLE));
  schema.add_attribute(Attribute("b", INT32, NOT_NULLABLE));
  cout << "foobar2" << endl;
  // Create the input container and define input size.
  View input_view(schema);
  input_view.set_row_count(row_count);

  // Insert the input columns into the view. The second argument of the Reset
  // method is an array of bools indicating which data positions should
  // be treated as null values. Passing a NULL as the array means there are
  // no null values.
  input_view.mutable_column(0)->Reset(a, NULL);
  input_view.mutable_column(1)->Reset(b, NULL);
  cout << "foobar2" << endl;
  // To receive the final evaluation result we call the bound expression's
  // Evaluate method on the input view. The EvaluationResult type is defined
  // as FailureOrReference<const View>, which works in a similar fashion
  // FailureOrOwned does, but it wraps up a reference rather than a pointer.
  EvaluationResult result = bound_tree->Evaluate(input_view);

  // We should now test for errors and return the data stored in the result
  // view's only column. A few more checks are run to be absolutely sure we've
  // got what we wanted.
  //  EXPECT_TRUE(result.is_success()) << result.exception().message();
  if( not result.is_success() )
    cout << result.exception().message() << endl;
  // Success cannot be failure.
  if( result.is_failure() )
    cout << result.exception().message() << endl;
  //EXPECT_FALSE(result.is_failure());

  // Get column number using view's column_count() method.
  //EXPECT_EQ(1, result.get().column_count());

  // And similarly for rows - we should have just as many as we started with.
  //EXPECT_EQ(row_count, result.get().row_count());
  cout << "foobar3" << endl;
  return result.get().column(0).typed_data<INT32>();
}

extern "C" {

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
  {"run", 0, run},
  {"new_table", 2, new_table},
  {"drop_table", 1, drop_table},
  {"add_row", 2, add_row},
  {"min", 2, min},
  {"max", 2, max}
};

BoundExpressionTree* PrepareBoundExpression() {
  std::unique_ptr<const Expression> addition(
      Plus(AttributeAt(0), AttributeAt(1)));
  // The Plus construction method is self-explanatory - it takes two
  // expressions and returns their sum. The AttributeAt function will provide
  // values from the input residing in the column at the given index. Note that
  // the indices are numbered starting from zero and do not necessarily have to
  // be consecutive numbers.
  //
  // We now describe the input's tuple schema by creating an empty one
  // and adding attributes (column descriptions). For each attribute the column
  // name, type and nullability have to be provided.
  TupleSchema schema;
  schema.add_attribute(Attribute("a", INT32, NOT_NULLABLE));
  schema.add_attribute(Attribute("b", INT32, NOT_NULLABLE));

  cout << "foobar" << endl;
  // Once we have both the expression and the schema we may bind them. In order
  // to do it we need an allocator which will take care of creating space
  // for results. A HeapBufferAllocator will take care of the job and allocate
  // heap memory using C routines. Unless you want to do some manual memory
  // management you should be happy using the HeapBufferAllocator as shown
  // below.
  FailureOrOwned<BoundExpressionTree> bound_addition =
    addition->Bind(schema, HeapBufferAllocator::Get(), 2048);
  
  cout << "foobar" << endl;

  // We have also provided a maximum result row count in a single block.
  // As the memory may be unavailable, the binding could fail. Supersonic does
  // not use plain C++ exceptions - it has its own mechanism for handling
  // undesirable operation outcomes. The FailureOrOwned object is a wrapper
  // which can tell us whether or not the operation has succeeded and provide
  // the result. Hopefully, in this simple scenario we will not run into memory
  // failures and we can access the result using release(). If there was
  // an error, .exception().message() would tell us more about it.
  if(not bound_addition.is_success())  {
    cout << bound_addition.exception().message() << endl;
    return NULL;
  }
  cout << "foobar" << endl;
  return bound_addition.release();
}


ERL_NIF_TERM run(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[])
{

  int32 a[8] = {0, 1, 2, 3,  4, 5, 6,  7};
  int32 b[8] = {3, 4, 6, 8,  1, 2, 2,  9};

  // Expected output array.
  int32 c[8] = {3, 5, 8, 11, 5, 7, 8, 16};

  // Go!
  std::unique_ptr<BoundExpressionTree> expr(PrepareBoundExpression());
  cout << expr.get()  << endl;
  const int32* result = AddColumns(a, b, 8, expr.get());
  cout << expr.get()  << endl;
  // Test results.
  for (int i = 0; i < 8; i++) {
    cout << c[i] <<  result[i] << "Error at position: " << i;
  }

  printf("hello\n");
  return enif_make_atom(env, "ok");
}

  ERL_NIF_TERM new_table(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[]){
    return enif_make_atom(env, "per");
  }
  ERL_NIF_TERM drop_table(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[]){
    return 0;
}
  ERL_NIF_TERM add_row(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[]){
    return 0;
  }
  ERL_NIF_TERM min(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[]){
    return enif_make_atom(env, "pew");
  }
  ERL_NIF_TERM max(ErlNifEnv* env, int argc,  const ERL_NIF_TERM argv[]){
    return enif_make_atom(env, "pew");
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

}
