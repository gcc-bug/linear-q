set(MQT_CORE_TARGET_NAME ${PROJECT_NAME}_core)

# main project library
add_library(
  ${MQT_CORE_TARGET_NAME}
  ${MQT_CORE_INCLUDE_BUILD_DIR}/CircuitOptimizer.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/Definitions.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/Expression.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/ClassicControlledOperation.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/CompoundOperation.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/Control.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/NonUnitaryOperation.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/Operation.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/StandardOperation.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/operations/SymbolicOperation.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Scanner.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Token.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Parser.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Statement.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Types.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/InstVisitor.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/NestedEnvironment.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Exception.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/Gate.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/StdGates.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/passes/CompilerPass.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/passes/ConstEvalPass.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/parsers/qasm3_parser/passes/TypeCheckPass.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/Permutation.hpp
  ${MQT_CORE_INCLUDE_BUILD_DIR}/QuantumComputation.hpp
  CircuitOptimizer.cpp
  operations/ClassicControlledOperation.cpp
  operations/Expression.cpp
  operations/NonUnitaryOperation.cpp
  operations/Operation.cpp
  operations/StandardOperation.cpp
  operations/SymbolicOperation.cpp
  parsers/GRCSParser.cpp
  parsers/QASM3Parser.cpp
  parsers/QCParser.cpp
  parsers/RealParser.cpp
  parsers/TFCParser.cpp
  parsers/qasm3_parser/Parser.cpp
  parsers/qasm3_parser/Scanner.cpp
  parsers/qasm3_parser/Types.cpp
  parsers/qasm3_parser/Statement.cpp
  parsers/qasm3_parser/passes/ConstEvalPass.cpp
  parsers/qasm3_parser/passes/TypeCheckPass.cpp
  QuantumComputation.cpp)

  # set include directories
  target_include_directories(
    ${MQT_CORE_TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${MQT_CORE_INCLUDE_BUILD_DIR}>
                                   $<INSTALL_INTERFACE:${MQT_CORE_INCLUDE_INSTALL_DIR}>)