#include "utils/helpers.hpp"

namespace duckdb {
namespace internal {

bool ShouldRun(TableFunctionInput &input) {
  auto state =
      dynamic_cast<RunOnceTableFunctionState *>(input.global_state.get());
  D_ASSERT(state != nullptr);
  if (state->run) {
    return false;
  }

  state->run = true;
  return true;
}

unique_ptr<FunctionData>
SingleStringResultBind(ClientContext &, TableFunctionBindInput &,
                       vector<LogicalType> &out_types,
                       vector<std::string> &out_names) {
  out_names.emplace_back("result");
  out_types.emplace_back(LogicalType::VARCHAR);
  return nullptr;
}

unique_ptr<FunctionData> SingleBoolResultBind(ClientContext &,
                                              TableFunctionBindInput &,
                                              vector<LogicalType> &out_types,
                                              vector<std::string> &out_names) {
  out_names.emplace_back("result");
  out_types.emplace_back(LogicalType::BOOLEAN);
  return nullptr;
}

} // namespace internal
} // namespace duckdb
