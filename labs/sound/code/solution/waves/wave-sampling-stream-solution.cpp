#include "solution/waves/wave-sampling-stream-solution.h"


namespace
{
    class WaveSamplingStream : public Stream<double>
    {
    private:
        Wave m_wave;
        unsigned m_sampling_rate;

    public:
        WaveSamplingStream(Wave wave, unsigned sampling_rate)
            : m_wave(wave), m_sampling_rate(sampling_rate) { }

        unsigned size() const override
        {
            return (unsigned)(m_wave.length() * m_sampling_rate);
        }

        double operator [](unsigned index) const override
        {
            return m_wave[((double)index) / m_sampling_rate];
        }
    };
}

std::shared_ptr<Stream<double>> solution::sample_wave(Wave wave, unsigned sampling_rate)
{
    return std::make_shared<WaveSamplingStream>(wave, sampling_rate);
}