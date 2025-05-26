#ifndef MBO_WAV_OUT_H
#define MBO_WAV_OUT_H

#include <array>
#include <cstdint>
#include <deque>
#include <fstream>
#include <ranges>

constexpr unsigned SAMPLE_RATE { 44100 };

constexpr unsigned AUDIO_DUMP_DURATION_S { 30 };
constexpr std::uint16_t BITS_PER_SAMPLE { 32 };
constexpr std::uint16_t BYTES_PER_SAMPLE { BITS_PER_SAMPLE / 8 };
constexpr std::uint16_t FMT_CHUNK_EXTRA_BYTES { 2 };

constexpr std::uint32_t mk_samples_per_channel(
    double duration_seconds, std::uint32_t sample_rate )
{
    return static_cast<std::uint32_t>(duration_seconds * sample_rate);
}

constexpr std::uint32_t mk_data_size(
    double duration_seconds, std::uint32_t sample_rate,
    std::uint16_t channels )
{
    return mk_samples_per_channel( duration_seconds, sample_rate)
            * channels * BYTES_PER_SAMPLE;
}

constexpr std::uint32_t mk_byte_rate(
    std::uint32_t sample_rate, std::uint16_t channels)
{
    return sample_rate * channels * BYTES_PER_SAMPLE;
}

constexpr std::uint16_t mk_block_align( std::uint16_t channels )
{
    return channels * BYTES_PER_SAMPLE;
}

constexpr std::uint32_t mk_riff_size(
    double duration_seconds, std::uint32_t sample_rate,
    std::uint16_t channels);

struct WavHeader {
    std::array<char, 4> riff_id {'R', 'I', 'F', 'F'};
    std::uint32_t riff_size;

    std::array<char, 4> wave_id {'W', 'A', 'V', 'E'};
    std::array<char, 4> fmt_id  {'f', 'm', 't', ' '};
    std::uint32_t fmt_size { 16 }; // PCM or IEEE float
    std::uint16_t audio_format { 3 }; // 3 = IEEE float
    std::uint16_t num_channels;
    std::uint32_t sample_rate;
    std::uint32_t byte_rate;
    std::uint16_t block_align;
    std::uint16_t bits_per_sample = BITS_PER_SAMPLE;

    std::array<char, 4> data_id {'d', 'a', 't', 'a'};
    std::uint32_t data_size;

    std::array<char, 4> fact_id {'f', 'a', 'c', 't'};
    std::uint32_t fact_size { 4 };
    std::uint32_t samples_per_channel;

    static constexpr WavHeader make(
            double duration_seconds,
            std::uint32_t sample_rate,
            std::uint16_t channels)
    {
        return WavHeader{
            .riff_size = mk_riff_size(duration_seconds, sample_rate, channels),
            .num_channels = channels,
            .sample_rate = sample_rate,
            .byte_rate = mk_byte_rate(sample_rate, channels),
            .block_align = mk_block_align(channels),
            .data_size = mk_data_size(duration_seconds, sample_rate, channels),
            .samples_per_channel
                = mk_samples_per_channel(duration_seconds, sample_rate)
        };
    }
};

constexpr std::uint32_t mk_riff_size(
    double duration_seconds, std::uint32_t sample_rate,
    std::uint16_t channels)
{
    return (sizeof(WavHeader) - 8)
              + mk_data_size(duration_seconds, sample_rate, channels);
}

// Base generic N-channel helper
template <std::size_t N>
struct deque_push_helper {
    template <typename... Args>
    static void push(std::deque<float>& dq, Args&... args) {
        static_assert(sizeof...(Args) == N, "Expected exactly N arguments");
        static_assert((std::conjunction_v<
                        std::is_convertible<Args, const float>...>),
                      "All arguments must be convertible to const float");

        (..., (dq.push_back(static_cast<const float>(args)),
               dq.pop_front()));
    }
};

// Specialization for stereo
template <>
struct deque_push_helper<2> {
    static void push(std::deque<float>& dq,
                     const float& left, const float& right) {
        dq.push_back(left);  dq.pop_front();
        dq.push_back(right); dq.pop_front();
    }
};

// Specialization for mono
template <>
struct deque_push_helper<1> {
    static void push(std::deque<float>& dq, const float& sample) {
        dq.push_back(sample);
        dq.pop_front();
    }
};

template <std::size_t N>
class WavOut {
public:
    using deque_t = std::deque<float>;

    WavOut( const char* output_filename, unsigned sample_rate = SAMPLE_RATE )
        : sample_rate( sample_rate )
        , deque( num_channels * duration_in_seconds * sample_rate, 0.0f )
    {
        out_file.open( output_filename, std::ios::binary );
        if (!out_file)
            throw std::ios_base::failure("Failed to open output file");

        // Write the .WAV header first
        WavHeader h = WavHeader::make( duration_in_seconds, sample_rate, N );
        out_file.write(reinterpret_cast<const char*>(&h), sizeof(WavHeader));

        // Pre-allocate empty sample data on disk.
        const float dummy { 0.0f };
        for (auto&& _ : std::views::iota( static_cast<unsigned>(0),
                     num_channels * duration_in_seconds * sample_rate )) {
            out_file.write( reinterpret_cast<const char*>(&dummy),
                            sizeof(dummy));
            if (!out_file)
                throw std::ios_base::failure("Failed to write to output file");
        }

       out_file.seekp(sizeof(WavHeader));

       if (!out_file)
           throw std::ios_base::failure("Failed to seek output file");

       out_file.flush();
       if (!out_file)
           throw std::ios_base::failure("Failed while flushing output file");
    }

    ~WavOut() {
        for (const auto& val : deque)
            out_file.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }

    template <typename... Args>
    void push_frame(const Args&... args) {
        deque_push_helper<N>::push(deque, args...);
    }

    static constexpr unsigned duration_in_seconds { AUDIO_DUMP_DURATION_S };
    static constexpr unsigned num_channels { N };
    const unsigned sample_rate;
private:
    deque_t deque;
    std::ofstream out_file;
};

#endif // MBO_WAV_OUT_H
