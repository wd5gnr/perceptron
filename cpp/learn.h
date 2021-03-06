
class Learn
{
protected:
  unsigned int stimct;
  unsigned int resct;
  float threshold;
  float **weights;
  float *result;
  float *_stim;  // place to put stim + bias
  void set_stim(float *s);
public:
  Learn(unsigned int stimulusct, unsigned int resultct);
  ~Learn();
  int init(void);
  int train(float *stim, unsigned int result, float wt=1.0f);
  unsigned int fetch(float *stim);
  void setThreshold(float t) { threshold=t; };
  float getThreshold(void) { return threshold; };
  int load(const char *fn);
  int save(const char *fn);
  void dump(void);
};
