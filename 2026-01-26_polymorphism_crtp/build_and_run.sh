#!/bin/bash
# Build and run all benchmarks with perf stats
# Requires: Google Benchmark (https://github.com/google/benchmark)
#
# Usage: ./run_benchmark.sh [repetitions]
#   repetitions: number of benchmark repetitions (default: 10)

set -e
cd "$(dirname "$0")"

CXX=${CXX:-clang++}
CXXFLAGS="-O2 -std=c++20 -march=native"
LDFLAGS="-lbenchmark"
PERF_EVENTS="cycles,instructions,cache-misses,branch-misses"

BENCH_REPS=${1:-10}
REPS="--benchmark_repetitions=$BENCH_REPS --benchmark_report_aggregates_only=true"

# Define benchmarks: "source_file:section_name:filter1,filter2,..."
BENCHMARKS=(
    "bench_direct:Direct:bmDirect"
    "bench_virtual:Virtual:bmVirtualDirect,bmVirtualHomogeneous,bmVirtualHeterogeneous"
    "bench_final:Final:bmFinalDirect,bmFinalHomogeneous,bmFinalHeterogeneous"
    "bench_crtp:CRTP:bmCrtp"
)

run() {
    echo "\$ $*"
    "$@"
}

run_perf() {
    local binary="$1"
    local filter="$2"
    echo "\$ perf stat \\"
    echo "    -e $PERF_EVENTS \\"
    echo "    $binary \\"
    echo "    --benchmark_repetitions=$BENCH_REPS \\"
    echo "    --benchmark_report_aggregates_only=true \\"
    echo "    --benchmark_filter=$filter"
    perf stat -e $PERF_EVENTS "$binary" $REPS --benchmark_filter="$filter"
}

echo "=== Building ==="
for entry in "${BENCHMARKS[@]}"; do
    src="${entry%%:*}"
    run $CXX $CXXFLAGS -o "$src" "$src.cpp" $LDFLAGS
done

echo ""
echo "=== Benchmarks with perf stats ==="
for entry in "${BENCHMARKS[@]}"; do
    src="${entry%%:*}"
    rest="${entry#*:}"
    section="${rest%%:*}"
    filters="${rest#*:}"
    
    echo ""
    echo "=== $section ==="
    
    IFS=',' read -ra filter_array <<< "$filters"
    for filter in "${filter_array[@]}"; do
        echo ""
        echo "--- $filter ---"
        run_perf "./$src" "$filter"
    done
done

echo ""
echo "=== Generating assembly ==="
for entry in "${BENCHMARKS[@]}"; do
    src="${entry%%:*}"
    run $CXX $CXXFLAGS -S "$src.cpp" -o "$src.s"
done
run $CXX $CXXFLAGS -S direct_standalone.cpp -o direct_standalone.s
echo "Assembly files: $(printf '%s.s ' "${BENCHMARKS[@]%%:*}") direct_standalone.s"
