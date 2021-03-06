/**
 ** Isaac Genome Alignment Software
 ** Copyright (c) 2010-2014 Illumina, Inc.
 ** All rights reserved.
 **
 ** This software is provided under the terms and conditions of the
 ** GNU GENERAL PUBLIC LICENSE Version 3
 **
 ** You should have received a copy of the GNU GENERAL PUBLIC LICENSE Version 3
 ** along with this program. If not, see
 ** <https://github.com/illumina/licenses/>.
 **
 ** \file KmerGenerator.hpp
 **
 ** A componant providing a simple way to iterate over a sequence and generate
 ** the corresponding kmer.
 **
 ** \author Come Raczy
 **/


#ifndef iSAAC_OLIGO_KMER_GENERATOR_HH
#define iSAAC_OLIGO_KMER_GENERATOR_HH

#include "oligo/Kmer.hh"
#include "oligo/Nucleotides.hh"

namespace isaac
{
namespace oligo
{

/**
 ** \brief A component to generate successive kmers from a sequence.
 **
 ** \param T the type of the kmer
 **/
template <unsigned kmerLength_, class T, typename InputIteratorT = std::vector<char>::const_iterator >
class KmerGenerator
{
public:
    /**
     ** \build a KmerGenerator for the given sequence.
     **
     ** \param begin the beginning of the sequence
     **
     ** \param end the end of the sequence (STL-like end)
     **
     ** param kmerLength the length of the kmers to produce.
     **/
    KmerGenerator(
        const InputIteratorT begin,
        const InputIteratorT end)
        : current_(begin)
        , end_(end)
        , mask_(~oligo::shlBases(T(~T(0)), kmerLength_))
        , kmer_(0)
    {
        const T one(1);
        assert((oligo::BITS_PER_BASE * kmerLength_) <= (8 * sizeof(T)));
        assert(!((oligo::shlBases(one, kmerLength_) & mask_)));
//        assert((oligo::shlBases(one, kmerLength_) - T(1)) == ((oligo::shlBases(one, kmerLength_) - T(1)) & mask_));
        initialize();
    }

    /**
     ** \brief Retrieve the next k-mer hat does not contain any N.
     **
     ** \param kmer the next kmer if any
     **
     ** \param start position of the produced kmer, if any
     **
     ** \return true if a kmer was produced. False otherwise (the end of the
     ** sequence has been reached)
     **/
    bool next(T &kmer, InputIteratorT &position)
    {
        static const Translator defaultTranslator = getTranslator();

        while ((current_ < end_) && (4 <= defaultTranslator[*current_]))
        {
            initialize();
        }
        if (current_ < end_)
        {
            kmer_ <<= 2;
            kmer_ |= defaultTranslator[*current_];
            kmer_ &= mask_;
            kmer = kmer_;
            ++current_;
            position = current_ - kmerLength_;
            return true;
        }
        else
        {
            return false;
        }
    }

    /**
     ** \brief Retrieve the next k-mer hat does not contain any N.
     **
     ** \param kmer the next kmer if any
     **
     ** \param start the position of the next kmer, if any
     **
     ** \return Iterator in the sequence for the position at which the kmer was produced. End iterator when no
     **         more kmers can be produced
     **/
    InputIteratorT next(T &kmer)
    {
        static const Translator defaultTranslator = getTranslator();

        while ((current_ < end_) && (INVALID_OLIGO <= defaultTranslator[*current_]))
        {
            initialize();
        }
        if (current_ < end_)
        {
            kmer_ <<= 2;
            kmer_ |= T(defaultTranslator[*current_]);
            kmer_ &= mask_;
            kmer = kmer_;
            ++current_;
            InputIteratorT position = current_ - kmerLength_;
            return position;
        }
        else
        {
            return end_;
        }
    }

private:
    InputIteratorT current_;
    const InputIteratorT end_;
    const T mask_;
    T kmer_;
    /// initialize the internal kmer_, skipping the Ns
    void initialize()
    {
        static const Translator defaultTranslator = getTranslator();

        unsigned currentLength = 0;
        while((current_ < end_) && currentLength + 1 < kmerLength_)
        {
            const unsigned baseValue = defaultTranslator[*current_];
            if(INVALID_OLIGO > baseValue)
            {
                kmer_ <<= 2;
                kmer_ |= T(baseValue);
                ++currentLength;
            }
            else
            {
                currentLength = 0;
                kmer_ = T(0);
            }
            ++current_;
        }
    }
};

template <typename KmerT>
inline KmerT getMaxKmer(const unsigned kmerLength)
{
    return ~(~KmerT(0) << 2 * kmerLength);
}

template <unsigned kmerLength, typename KmerT> struct MaxKmer
{
    static const unsigned value = ~(~KmerT(0) << 2 * kmerLength);
};

/**
 * \brief returns a kmer from the provided sequence.
 *
 * \return Returned kmer may contain Ns
 */
template <typename T, typename InputIteratorT>
bool generateKmer(
    unsigned kmerLength,
    T &kmer,
    InputIteratorT current,
    const InputIteratorT end)
{
    static const Translator defaultTranslator = getTranslator();

    for (unsigned todo = kmerLength; todo; --todo, ++current)
    {
        if (current == end)
        {
            return false;
        }
        kmer <<= 2;
        kmer |= defaultTranslator[*current];
    }
    kmer &= (~((~T(0)) << (2 * kmerLength)));
    return true;
}


} // namespace oligo
} // namespace isaac

#endif // #ifndef iSAAC_OLIGO_KMER_GENERATOR_HH
