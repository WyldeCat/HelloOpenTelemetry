#include "opentelemetry/exporters/ostream/span_exporter_factory.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <random>


namespace trace_api = opentelemetry::trace;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace trace_exporter = opentelemetry::exporter::trace;

namespace {
  void InitTracer() {
    auto exporter  = trace_exporter::OStreamSpanExporterFactory::Create();
    auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));
    std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
      trace_sdk::TracerProviderFactory::Create(std::move(processor));
    //set the global trace provider
    trace_api::Provider::SetTracerProvider(provider);
  }
  void CleanupTracer() {
    std::shared_ptr<opentelemetry::trace::TracerProvider> none;
    trace_api::Provider::SetTracerProvider(none);
  }
}

int64_t get_random_duration_us() {
  return 10 + (std::rand() % 1000);
}

int64_t get_current_time_us() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();
}

void produce_past_logs() {
  auto tracer = opentelemetry::trace::Provider::GetTracerProvider()->GetTracer("my-app-tracer");

  while (true) {
    const int64_t duration = get_random_duration_us();

    opentelemetry::common::SteadyTimestamp start;
    std::this_thread::sleep_for(std::chrono::microseconds(duration));
    opentelemetry::common::SteadyTimestamp end;

    opentelemetry::trace::StartSpanOptions st;
    st.start_steady_time = start;

    opentelemetry::trace::EndSpanOptions ed;
    ed.end_steady_time = end;

    tracer->StartSpan("[T2] Log", st)->End(ed);
  }
}

void produce_logs() {
  auto tracer = opentelemetry::trace::Provider::GetTracerProvider()->GetTracer("my-app-tracer");

  while (true) {
    const int64_t duration = get_random_duration_us();

    auto span = tracer->StartSpan("[T1] Log");
    std::this_thread::sleep_for(std::chrono::microseconds(duration));
    span->End();
  }
}

int main() {
  InitTracer();

  std::thread producer(produce_past_logs);
  produce_logs();

  CleanupTracer();
  return 0;
}
