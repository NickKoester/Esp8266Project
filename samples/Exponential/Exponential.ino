
//======= GENERATOR PARAMETERS ======//
const double INTERVAL = 5.0;
const int RANDOM_SEED = 1;
const int RANDOM_MAX = 100000;

//======== TESTING PARAMETERS =======//
const int SAMPLE_SIZE = 10000;

double sum = 0;
int i = 0;

void testAverage(const double val)
{
  if(i < SAMPLE_SIZE) {
    Serial.println(val);
    Serial.println();
    
    sum += val / SAMPLE_SIZE;
    i++;
  }
  else {
    Serial.println("==============");
    Serial.print("Mean : ");
    Serial.println(sum);
    delay(10000);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  srand(RANDOM_SEED);
}

double generateExponential()
{
  double u = rand() / (double) RAND_MAX;
  return -INTERVAL * log(u);
}

void loop() {
  if(i < 1000) {
    double num = generateExponential();
    Serial.println(num);
    i++;
  }
  else{
    delay(10000);
  }
}
