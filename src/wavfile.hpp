#ifndef WAVFILE_H
#define WAVFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <float.h>

#include <sndfile.h>

#define BLOCK_SIZE 512

class WavFile
{
public:
  WavFile() {
    if ((infile = sf_open ("in.wav", SFM_READ, &sfinfo)) == NULL) {
      printf ("Not able to open input file %s.\n", "wav") ;
      //puts (sf_strerror (NULL)) ;
    } else {

      buffer_size = 0;
      buffer = new double[3000000];
      convert_to_text (infile, sfinfo.channels) ;
      std::cout << buffer_size << "\n";
    }
  }

  void convert_to_text (SNDFILE * infile, int channels) {
    float buf [channels * BLOCK_SIZE] ;
    int k, m, readcount ;
    int bi = 0;
    while ((readcount = sf_readf_float (infile, buf, BLOCK_SIZE)) > 0)
    { for (k = 0 ; k < readcount ; k++)
      { for (m = 0 ; m < channels ; m++)
          // printf (" % 12.10f", buf [k * channels + m]) ;
          // printf ("\n") ;
          buffer[bi] = buf [k * channels + m];
          bi++;
        };
      };
      buffer_size = bi;



    return ;
  }


  static void write(std::vector<double> &music)
  {
    int SAMPLE_RATE = 44100;
    SNDFILE *file ;
    SF_INFO sfinfo ;
    int   k ;

    memset (&sfinfo, 0, sizeof (sfinfo)) ;

    double max = 0;
    for (int i=0;i<music.size();i++)
      if (fabs(music[i]) > max)
        max = fabs(music[i]);

    for (int i=0;i<music.size();i++)
      music[i] /= max;

    sfinfo.samplerate = SAMPLE_RATE ;
    sfinfo.frames   = music.size() ;
    sfinfo.channels   = 1 ;
    sfinfo.format   = (SF_FORMAT_WAV | SF_FORMAT_DOUBLE) ;

    if (! (file = sf_open ("out.wav", SFM_WRITE, &sfinfo)))
    { printf ("Error : Not able to open output file.\n") ;
      return ;
      } ;

    if (sf_write_double (file, music.data(), music.size()) !=
                        music.size())
      puts (sf_strerror (file)) ;

    sf_close (file) ;
    return  ;
  }

  SNDFILE   *infile;
  SF_INFO   sfinfo ;
  double *buffer;
  int buffer_size;
};

#endif
