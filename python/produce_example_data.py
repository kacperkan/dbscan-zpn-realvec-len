from pathlib import Path
from typing import Tuple

import click
import matplotlib.pyplot as plt
import numpy as np

data = """
4.489013,3.949385,2
3.234519,3.032182,2
2.339381,3.233512,2
3.067888,3.272515,2
3.337977,3.214286,2
1.623692,1.968673,2
4.459075,3.803079,2
2.746071,2.881115,2
3.022935,3.244224,2
2.634098,2.410794,2
2.209555,2.269341,2
2.93852,4.243461,2
3.307581,3.077136,2
4.325037,3.338618,2
2.868755,2.639683,2
2.849528,2.990066,2
3.360591,3.146077,2
1.935163,2.120289,2
3.001328,3.005997,2
2.815287,2.893017,2
1.970595,2.479919,2
3.990677,3.728278,2
3.402707,3.298334,2
3.945814,2.965621,2
2.735176,2.44998,2
3.608432,3.170064,2
3.252739,3.002152,2
2.484497,3.060198,2
3.994705,3.673803,2
2.799814,2.734718,2
2.907117,2.647374,2
3.115223,4.262139,2
2.973952,2.696356,2
3.986648,3.551485,2
3.326075,3.389431,2
1.859722,1.628361,2
2.762185,2.475341,2
2.888989,3.010208,2
2.817301,3.156148,2
3.017533,2.841655,2
3.685522,3.447386,2
3.414609,3.078967,2
2.412534,2.670171,2
3.551576,3.318201,2
3.1198,3.169698,2
3.662816,3.273339,2
1.914106,2.575411,2
2.720985,2.346248,2
3.747047,3.591311,2
4.447722,3.787515,2
2.788827,2.815561,2
3.065783,2.989334,2
3.015519,3.216941,2
2.533937,2.938429,2
2.929548,3.161733,2
3.606601,3.310785,2
3.96147,3.833018,2
3.200095,3.34109,2
3.943892,3.325709,2
2.584658,2.652593,2
3.367641,3.577944,2
2.17513,2.389279,2
3.093066,3.12978,2
2.019761,2.741035,2
4.233482,4.22158,2
2.910321,2.896588,2
1.983322,2.480926,2
3.507996,2.27694,2
3.990402,4.059343,2
1.925916,1.847545,2
3.645787,3.182974,2
4.467864,3.664098,2
3.275536,3.071001,2
4.161977,3.177847,2
3.09508,2.91792,2
4.149342,2.954726,2
3.058275,2.963149,2
3.212088,3.750984,2
2.805124,2.793954,2
4.023911,3.317652,2
1.784738,2.587313,2
2.851543,2.983566,2
3.440977,3.477416,2
3.884014,3.279107,2
2.566988,1.667821,1
3.63187,3.683508,2
4.088458,3.490051,2
3.419828,3.19982,2
1.620579,2.594363,2
2.707251,2.4476,2
3.235984,3.104053,2
3.289636,3.267296,2
2.4129,2.971023,2
3.734321,3.365169,2
3.03209,3.086383,2
3.335414,2.976608,2
3.027696,2.906568,2
4.230003,3.549654,2
2.794412,2.961592,2
2.394681,2.819956,2
3.058641,2.859142,2
4.406613,3.409665,2
2.688025,2.837901,2
3.278008,2.598025,2
3.077044,3.235527,2
2.397153,2.696722,2
3.55121,3.163015,2
4.190451,3.168416,2
3.975661,3.483825,2
2.23281,2.231712,2
3.067614,2.632267,2
4.437742,3.717017,2
3.95735,3.623539,2
1.619938,2.30633,2
2.358333,2.488342,2
2.216697,3.00325,2
1.918134,2.094745,2
4.448454,3.367367,2
4.092486,3.572451,2
3.174734,3.003067,2
2.684088,3.011216,2
2.024979,1.756264,2
3.234336,3.186087,2
2.188406,3.151204,2
3.201285,3.398129,2
3.079424,3.077868,2
3.771584,3.194418,2
3.158528,4.194937,2
3.240745,2.722175,2
3.004074,2.656713,2
4.359004,3.983718,2
3.045457,3.725257,2
3.311518,3.196341,2
3.102863,3.114399,2
2.70423,2.82728,2
2.041917,2.068468,2
3.529603,3.167592,2
1.794351,2.203787,2
3.494263,3.562563,2
4.08269,3.480621,2
2.457213,3.019456,2
3.813425,3.711249,2
3.066973,3.531343,2
2.773629,2.88963,2
2.249748,2.807321,2
3.618595,3.175283,2
2.797983,2.84019,2
1.904126,3.045457,2
3.362239,2.910504,2
2.943556,2.240318,2
2.555361,2.811625,2
3.765175,3.232414,2
2.491638,2.394314,2
3.685614,2.645451,2
3.340632,3.25988,2
1.918226,2.746071,2
2.996475,3.052416,2
2.523499,3.386685,2
3.628666,3.390255,2
4.116474,3.218497,2
2.613132,2.772439,2
3.207785,3.330836,2
2.992813,3.088488,2
3.233055,3.377255,2
1.7678,2.044023,2
2.996384,3.115223,2
3.60184,2.84376,2
3.942701,3.336421,2
3.245048,3.634434,2
2.772988,3.315912,2
4.425474,3.311792,2
3.493622,3.076952,2
3.58655,3.219779,2
3.369289,3.120167,2
3.822855,3.334498,2
2.712561,2.337275,2
3.655492,3.177847,2
2.587588,2.708991,2
3.208426,3.031816,2
3.306482,3.462127,2
3.516236,3.21337,2
2.43881,2.723365,2
3.334407,3.013596,2
4.029862,3.311426,2
3.439787,3.152211,2
3.083178,3.240837,2
2.100421,1.732002,2
3.254204,3.328455,2
2.785348,3.730201,2
4.286676,3.583987,2
1.88966,2.654149,2
2.709815,2.390561,2
2.923139,2.828928,2
2.654332,2.930189,2
2.894391,2.502533,2
3.320032,3.080798,2
2.326014,2.40228,2
1.332209,0.848247,4
0.332606,1.522279,4
1.871746,0.571383,4
0.207541,1.269402,4
1.274987,0.755776,4
-0.315058,1.648808,4
0.344783,0.985763,4
1.361049,0.866649,4
2.266167,0.112964,4
1.621342,0.739845,4
1.357845,0.763741,4
0.229057,1.541688,4
0.090167,2.268548,4
0.854747,0.964064,4
1.111011,0.955916,4
0.646733,1.228477,4
0.478088,1.719123,4
2.260125,0.195547,4
1.244407,0.994827,4
0.88432,1.153676,4
1.209342,0.657262,4
1.785043,0.957106,4
0.503357,1.339991,4
0.797342,1.06972,4
-0.031785,1.403989,4
1.447295,0.832316,4
0.872784,1.564486,4
0.167989,1.615665,4
2.123798,1.055895,4
0.881756,1.013138,4
1.551759,0.457488,4
1.230125,0.906842,4
0.793588,0.533113,4
1.406369,0.729133,4
1.399228,1.232414,4
0.842662,1.141316,4
-0.146321,1.08446,4
2.246941,0.728401,4
1.768929,0.250023,4
0.961959,0.926069,4
0.312922,1.584811,4
0.590243,1.318567,4
1.702002,0.393399,4
0.13329,0.976333,4
1.409116,0.916822,4
0.187765,1.056169,4
0.675939,0.721442,4
1.467528,0.469115,4
0.868755,0.925977,4
0.826731,1.108997,4
2.161336,0.437346,4
1.886944,0.347163,4
0.483856,1.694678,4
1.781289,0.356319,4
0.980819,0.936048,4
1.145161,0.52414,4
0.166158,1.298975,4
-0.324396,1.584536,4
-0.341426,1.224631,4
1.225364,1.003067,4
-0.391873,1.768654,4
0.487243,1.501587,4
0.555727,0.266137,4
1.35638,0.913251,4
1.426054,0.948958,4
0.37701,1.230766,4
1.322687,0.881298,4
1.500122,1.665929,4
0.299554,1.255211,4
0.599673,1.257958,4
1.203757,1.232139,4
-0.097888,1.645146,4
1.097736,1.046007,4
0.911328,1.066057,4
1.00911,0.979904,4
0.693609,0.765481,4
1.398953,1.09151,4
1.445647,1.087573,4
1.269585,1.145619,4
2.035539,-0.051561,4
-0.315882,2.136799,4
1.404996,1.620792,4
0.083392,1.695502,4
-0.300501,1.673162,4
1.384487,1.113392,4
0.2828,1.316553,4
0.106098,1.694678,4
0.322077,1.72956,4
1.707404,0.832774,4
1.344844,1.204581,4
0.537049,0.909223,4
2.113269,0.14922,4
0.928449,0.961592,4
1.793008,0.09273,4
0.832316,1.120258,4
0.886059,0.724006,4
0.6601,0.851817,4
0.420316,1.373867,4
0.638127,1.330836,4
1.000961,0.957747,4
0.699927,1.211447,4
0.949599,0.947493,4
1.890973,0.519196,4
1.723426,0.720435,4
1.51587,1.256035,4
1.186544,1.440428,4
0.507019,1.176748,4
0.225944,1.425779,4
1.159719,1.032456,4
1.120533,0.497131,4
0.635289,1.04152,4
1.659612,0.721809,4
0.931745,0.951613,4
0.488891,1.793649,4
0.992538,0.840556,4
-0.456511,1.874676,4
0.868572,0.742409,4
0.92909,1.108539,4
1.931257,0.60004,4
1.494903,0.540437,4
1.755745,0.369594,4
1.141133,0.991714,4
0.83433,1.098834,4
1.517609,1.001968,4
0.964156,0.889355,4
1.650548,0.798074,4
0.784982,0.945753,4
1.296777,0.94264,4
0.968551,1.134449,4
0.8661,1.078875,4
0.989883,1.458098,4
0.12862,1.467162,4
0.668523,0.963973,4
1.142232,0.816568,4
1.325159,0.839824,4
1.322321,0.588687,4
1.317011,0.877911,4
0.95674,1.112201,4
1.279565,0.934767,4
1.10799,0.923963,4
0.078906,1.856182,4
0.726112,1.119892,4
0.396146,1.256127,4
0.209189,0.86079,4
1.167959,1.00032,4
0.1657,1.553682,4
0.695715,1.142964,4
2.241081,0.369778,4
2.200613,-0.093585,4
-0.04268,1.140584,4
0.919202,0.842845,4
0.003372,1.514771,4
0.407498,0.82197,4
0.755226,1.210349,4
0.615238,1.132618,4
0.796884,1.184347,4
1.625828,0.80137,4
1.418088,0.815744,4
-0.177541,2.055132,4
-0.089007,1.580782,4
0.308618,0.921308,4
-0.428312,2.089282,4
1.589938,0.716224,4
1.155416,1.310327,4
0.352107,1.350429,4
0.088153,1.73075,4
1.129231,0.751839,4
2.048448,0.202872,4
1.430448,0.759346,4
2.012558,-0.068682,4
1.058275,0.115894,4
0.677679,1.285607,4
1.929609,0.365474,4
0.366573,1.350063,4
2.025559,0.646733,4
1.722327,0.960768,4
1.65128,0.438627,4
1.366817,0.796793,4
1.07503,0.974319,4
2.290521,0.317774,4
0.984756,0.973678,4
0.107013,2.15685,4
0.52121,1.250175,4
0.947493,1.286615,4
0.20159,1.429441,4
1.868816,0.062059,4
0.665868,1.041154,4
1.55945,0.707068,4
0.366573,1.778634,4
1.060381,1.00032,4
1.044176,1.323969,4
1.299524,0.697089,4
1.588565,0.387906,4
0.461058,0.879101,4
0.630345,1.248436,4
1.538026,0.2687,4
2.027299,0.041917,4
1.413785,0.778665,4
1.009568,0.959944,4
-0.051469,1.881542,4
0.885128,3.970916,3
0.515793,3.084201,3
0.982452,4.144597,3
0.128422,3.43614,3
-0.030885,3.130161,3
0.209265,3.584185,3
0.453169,3.568438,3
0.616688,3.896664,3
0.276742,3.416913,3
0.087405,3.86993,3
-0.271126,3.524674,3
0.844386,4.075198,3
0.390912,3.153325,3
0.971007,3.939055,3
1.48674,3.036866,3
-0.009552,2.54677,3
-0.074374,3.485946,3
0.308237,4.558061,3
0.47972,4.007172,3
0.964141,3.105716,3
0.878994,3.106906,3
0.497665,3.329203,3
0.480087,4.149724,3
0.20661,3.510575,3
1.151738,3.573565,3
0.04181,3.046205,3
-0.4373,3.074496,3
-0.195502,4.044252,3
0.768395,3.269051,3
1.335032,3.786157,3
0.3249,3.628132,3
0.338816,3.429365,3
0.416639,2.573504,3
0.532548,3.536485,3
0.408307,2.782891,3
0.612934,4.453139,3
-0.213355,4.576281,3
-0.685598,3.784326,3
1.17362,3.855739,3
0.335246,2.662496,3
0.62273,3.404187,3
1.207221,3.432844,3
0.519822,3.331584,3
0.507096,3.156346,3
-0.685415,4.379803,3
0.254677,4.889859,3
-0.051576,3.126133,3
1.586352,3.733512,3
1.144322,3.073489,3
1.82403,4.20539,3
0.07065,4.699057,3
0.013611,2.65041,3
1.365337,3.597186,3
-0.645497,3.334971,3
0.166051,3.200842,3
-0.453139,2.968017,3
1.00882,3.778558,3
0.166417,3.463973,3
0.410779,4.696951,3
0.604968,3.457472,3
0.715934,3.486221,3
0.057466,3.502335,3
0.113956,3.519547,3
1.234596,3.425703,3
0.120273,3.055086,3
0.899319,3.914151,3
0.415448,3.84695,3
0.562761,3.103702,3
-0.543504,3.972198,3
0.487869,2.824,3
-0.583239,3.318125,3
0.910215,4.157231,3
0.235176,3.721793,3
-0.126469,2.821894,3
1.250435,3.059481,3
0.823511,3.54857,3
1.797113,3.997833,3
0.319498,3.042543,3
0.79989,2.914457,3
0.447676,3.159459,3
-0.214637,2.469314,3
0.084017,4.152745,3
0.190588,2.855861,3
-0.450392,3.270608,3
1.898007,3.726096,3
-0.35847,3.083651,3
-0.353252,3.164586,3
0.689474,3.431013,3
0.253853,3.002716,3
0.558641,3.082369,3
0.898312,4.010743,3
0.475784,3.525681,3
0.459304,4.237709,3
1.202185,3.56624,3
0.382855,3.86462,3
0.100772,4.439222,3
0.463424,3.516526,3
0.476974,3.143345,3
1.079684,3.525132,3
0.374615,2.795892,3
-0.538011,3.651753,3
-0.383831,3.331034,3
1.055696,4.334391,3
0.993896,4.157872,3
0.275826,3.702109,3
0.389904,3.681509,3
0.464522,3.088961,3
-0.666829,3.808039,3
0.837794,3.58858,3
-0.698599,2.755791,3
0.458205,3.403272,3
0.729392,3.981536,3
0.313547,2.873348,3
-0.157964,3.197363,3
0.325907,3.4485,3
-0.121708,4.584704,3
1.381909,3.329295,3
1.270943,3.093448,3
0.49501,3.511124,3
0.610736,3.299173,3
-0.62508,3.143712,3
0.497482,3.74514,3
-0.131779,3.730399,3
0.305124,3.563952,3
0.940519,3.009491,3
-0.169042,3.39494,3
0.239021,4.161718,3
0.185186,2.893033,3
-0.734306,2.758721,3
-0.049837,3.139409,3
-0.332377,2.830409,3
0.478805,2.878933,3
-0.077303,3.415998,3
0.457472,4.719565,3
-0.040132,3.19837,3
-0.27662,4.49031,3
-0.228278,3.518357,3
0.202948,3.230873,3
0.536485,4.103214,3
0.495193,3.35136,3
0.347514,3.261269,3
0.010224,3.607532,3
1.292184,3.304483,3
1.554765,3.235084,3
-0.152104,3.453536,3
0.277291,4.95468,3
-0.342265,4.141942,3
1.771569,3.518723,3
0.856288,3.551775,3
-0.225623,3.630146,3
0.803644,3.704306,3
1.12418,3.071383,3
0.311533,3.328471,3
1.154851,3.234169,3
0.700369,3.5862,3
0.023591,3.808588,3
0.72454,3.667592,3
0.784417,3.86755,3
0.689566,2.997131,3
0.101596,4.1348,3
0.27546,3.141057,3
0.783776,3.478988,3
1.227454,3.224738,3
-0.328715,3.70101,3
0.569903,3.740196,3
0.016449,3.377819,3
-0.155217,4.657857,3
0.940153,3.380291,3
-0.259682,2.703055,3
-0.327708,4.99002,3
1.066683,4.00763,3
0.628315,3.307047,3
0.431745,3.225471,3
-0.742821,2.961425,3
0.155522,3.770318,3
1.997436,4.338694,3
0.951415,3.466628,3
0.533647,3.069552,3
-0.196966,3.260994,3
3.36E-4,4.792535,3
0.532273,3.239113,3
0.403729,3.725913,3
3.0466,0.8906,1
3.4236,1.2917,1
2.9544,1.5989,1
2.6431,1.2687,1
3.1879,1.1276,1
2.77,1.2251,1
2.6802,1.024,1
3.1576,1.456,1
3.3111,1.3991,1
2.7581,1.0598,1
2.5632,1.3539,1
3.4348,1.361,1
3.1107,0.9554,1
3.2574,1.2387,1
2.6672,1.4757,1
2.9605,0.7438,1
3.3683,0.9736,1
2.8829,0.8942,1
2.8578,1.0018,1
3.2157,1.6215,1
3.0573,0.9584,1
2.9994,1.1129,1
2.9966,0.8444,1
2.686,1.3302,1
2.8344,0.9064,1
3.0478,0.7252,1
3.2119,1.3143,1
2.8879,1.012,1
3.1902,0.8477,1
2.6245,1.1839,1
3.0361,1.4348,1
2.5609,1.0648,1
3.2659,1.3763,1
2.8736,1.4217,1
2.9743,1.5466,1
3.3924,1.0375,1
2.705,1.5801,1
2.739,1.3941,1
3.0772,0.9015,1
3.0222,0.9066,1
2.8963,1.2059,1
3.1287,1.2452,1
3.3168,1.4412,1
2.8625,1.5371,1
3.4016,1.3861,1
3.2661,1.5486,1
3.0381,0.776,1
3.0433,1.6482,1
3.4306,1.3829,1
3.3411,1.2587,1
2.99,1.6495,1
2.971,1.3644,1
3.1535,1.3853,1
3.4215,0.9532,1
2.891,0.9514,1
3.1124,0.9946,1
3.1296,0.936,1
2.9762,1.2447,1
2.8389,1.2302,1
3.3215,0.9276,1
2.6176,0.9427,1
3.0725,1.4201,1
3.3068,1.5678,1
2.921,1.4005,1
3.1617,0.8556,1
3.0239,1.1939,1
3.1172,1.3509,1
2.9652,0.8766,1
3.2834,0.8226,1
2.8343,1.4653,1
2.7326,1.2946,1
2.7182,1.5419,1
3.3502,1.5552,1
3.4764,1.2695,1
2.9481,1.5638,1
2.82,1.0248,1
3.133,0.7919,1
3.2613,0.844,1
2.7627,1.3866,1
2.6678,1.1849,1
3.0895,0.8952,1
2.9198,1.5528,1
2.813,0.7473,1
3.3119,1.0346,1
2.9812,1.3445,1
3.2469,1.0035,1
2.7491,0.8337,1
3.1573,1.2665,1
2.9082,0.7462,1
3.099,0.7124,1
3.1364,1.5234,1
3.3361,1.5093,1
3.2017,1.4367,1
2.9375,0.7484,1
3.4003,1.432,1
3.3817,1.1208,1
3.0486,1.358,1
2.6941,1.4811,1
3.2903,1.1851,1
2.8224,1.5228,1
3.2368,0.8982,1
3.1496,0.8235,1
3.3555,1.0175,1
3.0348,1.2413,1
3.1649,1.1417,1
2.7659,1.3498,1
3.383,1.2544,1
2.6798,1.2099,1
2.9077,0.9295,1
2.861,1.1694,1
2.633,1.1877,1
3.178,1.0399,1
3.2835,0.8457,1
2.607,1.1245,1
2.8487,0.9091,1
2.9784,1.5921,1
2.6954,1.0659,1
3.3844,1.2377,1
3.1864,1.3007,1
3.2634,0.942,1
2.9609,1.3483,1
2.3222,4.7297,0
2.0102,5.8892,0
1.9736,4.9542,0
2.0739,6.1202,0
1.8746,5.4656,0
1.8312,5.9963,0
2.7406,5.6412,0
2.3835,5.1521,0
1.5146,5.1934,0
1.8953,5.4618,0
2.7591,5.9729,0
2.0065,4.8304,0
1.7675,5.7072,0
2.3688,4.9301,0
2.6894,5.8566,0
2.6825,5.1377,0
2.2968,5.8287,0
1.4757,5.8335,0
1.472,5.7901,0
2.2275,4.8616,0
1.8202,6.1757,0
2.3617,5.8219,0
2.3221,5.5861,0
2.8457,5.1191,0
2.3828,6.0148,0
1.7969,5.2553,0
2.2738,6.1126,0
1.77,5.6628,0
2.5319,5.9331,0
1.552,5.3145,0
1.5312,5.213,0
2.7981,4.9891,0
2.4918,5.8279,0
2.4553,4.8288,0
1.7916,5.0269,0
1.637,4.9325,0
2.5627,5.9182,0
2.306,5.5763,0
1.9065,6.0425,0
1.4253,5.3605,0
2.069,5.5083,0
1.8674,5.2289,0
1.8638,5.8712,0
1.6409,6.0213,0
2.4964,4.9606,0
1.5325,5.214,0
1.7782,5.4711,0
2.3946,6.1663,0
2.4909,4.7728,0
2.6288,6.0937,0
2.2467,4.9457,0
1.993,5.4824,0
2.4213,5.7468,0
1.8352,5.7537,0
2.7662,5.6801,0
1.9225,5.7694,0
2.3213,5.6617,0
2.1368,5.1172,0
2.4768,5.4474,0
2.6968,6.0274,0
2.7557,6.0069,0
2.1598,4.8701,0
2.9752,5.5123,0
1.6072,5.6647,0
2.3178,6.1736,0
1.7799,5.1267,0
2.8634,5.3434,0
2.4801,5.3631,0
2.561,5.0926,0
1.8236,6.0521,0
2.3175,5.7688,0
2.6358,5.8178,0
2.5086,5.847,0
1.6335,5.8464,0
2.4169,5.8333,0
1.4211,5.4334,0
1.6612,5.9826,0
2.0288,6.0047,0
1.6625,5.02,0
2.3545,5.5553,0
1.6431,4.9918,0
2.5684,5.2849,0
2.0127,5.2208,0
2.1003,6.1335,0
2.1526,5.5316,0
1.5818,5.87,0
2.1452,4.8742,0
1.4964,5.5526,0
2.0921,5.531,0
2.2299,6.1897,0
2.3661,5.8457,0
2.1353,5.1438,0
1.9777,5.0026,0
2.1738,4.9302,0
1.6812,6.0884,0
2.665,5.8623,0
2.1855,5.2807,0
2.8673,5.6003,0
2.7301,5.0126,0
2.6533,5.1737,0
1.7884,5.7296,0
2.2207,5.5175,0
1.8122,5.6868,0
2.8111,5.358,0
1.9665,6.1298,0
2.0781,5.4653,0
2.3935,6.1625,0
2.0251,5.9905,0
1.5333,5.5441,0
1.9848,5.3651,0
2.6955,5.3894,0
2.0669,5.7137,0
2.7647,5.457,0
1.9887,4.8114,0
2.4028,5.2774,0
2.4226,5.2334,0
2.266,5.6961,0
2.2259,5.2215,0
2.64,5.749,0
1.9013,5.02,0
1.53,5.7113,0
2.2673,5.3282,0
2.247,5.3851,0
1.5879,5.5112,0
2.1713,5.0141,0
2.0475,5.0482,0
2.886,5.5345,0
2.6161,5.2967,0
2.6257,5.3263,0
1.4845,5.7957,0
1.518,5.1085,0
2.115,5.0725,0
2.9158,5.6042,0
2.0694,5.5139,0
2.8569,5.6597,0
2.4257,4.7493,0
2.7371,5.4397,0
2.8263,5.435,0
2.781,5.446,0
2.7076,5.7135,0
2.302,5.6704,0
2.0946,6.1397,0
1.8825,5.1417,0
2.1004,5.488,0
1.4692,5.3988,0
2.589,5.9952,0
2.2515,6.0501,0
1.4673,5.2398,0
2.656,5.9093,0
"""


def to_arff(data: np.ndarray, labels: np.ndarray) -> str:
    lines = [
        "% Data from J. Handl",
        "%",
        "%",
        "@RELATION 2d-20c-no0",
        "",
        "@ATTRIBUTE x REAL",
        "@ATTRIBUTE y REAL",
        "@ATTRIBUTE class {-1, 0, 1, 2}",
        "",
        "@DATA",
    ]
    for coords, lab in zip(data, labels):
        lines.append("{:.5f},{:.5f},{:d}".format(coords[0], coords[1], lab))
    return "\n".join(lines)


def get_even_simpler_dataset() -> Tuple[np.ndarray, np.ndarray]:
    left_points, right_points = [], []
    num_samples = 20
    y_coords = np.linspace(-1, 1, num=num_samples // 2)
    labels = [0] * (num_samples // 2) + [1] * (num_samples // 2)
    for i in range(num_samples // 2):
        left_points.append((-1.0, y_coords[i]))
        right_points.append((1.0, y_coords[i]))

    return (
        np.concatenate(
            (np.array(left_points), np.array(right_points)), axis=0
        ),
        np.array(labels),
    )


def produce(out: str):
    plt.switch_backend("agg")
    # Generate sample data
    out_path = Path(out)
    out_path.mkdir(parents=True, exist_ok=True)

    X = []
    labels_true = []
    for line in data:
        if len(line) == 0:
            continue
        x1, x2, label = line.split(",")
        X.append((float(x1), float(x2)))
        labels_true.append(int(label))

    with open(out_path / "data.arff", "w") as f:
        f.write(to_arff(np.array(X), np.array(labels_true)))

    simpler_path = out_path.parent / (out_path.name + "_simpler")
    simpler_path.mkdir(parents=True, exist_ok=True)

    X, labels_true = get_even_simpler_dataset()

    with open(simpler_path / "data.arff", "w") as f:
        f.write(to_arff(X, labels_true))


@click.command()
@click.argument("out_path")
def main(out_path: str):
    produce(out_path)


if __name__ == "__main__":
    main()
