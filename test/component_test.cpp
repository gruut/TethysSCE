#define BOOST_TEST_MODULE component_test

#include <iostream>
#include <boost/test/unit_test.hpp>

#include "../src/config.hpp"
#include "../src/condition/condition_manager.hpp"
#include "../src/data/data_manager.hpp"

using namespace tethys::tsce;

BOOST_AUTO_TEST_SUITE(test_compare_handler)
BOOST_AUTO_TEST_CASE(simple_compare) {

  std::string compare_condition = R"(
<condition id="test">
  <compare src="$tt" ref="$tt2" type="EQ" />
</condition>
)";

  pugi::xml_document doc;

  if (doc.load_string(compare_condition.c_str(), pugi::parse_minimal)) {
    pugi::xml_node doc_node = doc.document_element();

    ConditionManager condition_manager;
    DataManager data_manager;

    data_manager.updateValue("$tt","20");
    data_manager.updateValue("$tt2","20");

    BOOST_TEST(condition_manager.evalue(doc_node,data_manager));

  } else {
    BOOST_TEST(false);
  }
}

BOOST_AUTO_TEST_CASE(simple_endorser) {

  std::string endorser_condition = R"(
<condition id="test">
  <endorser>
    <if eval-rule="or">
      <id>bt1fDSTLzc1AdSoxXTy6bfAG5NZFHJBThEHjjWYHN2H</id>
      <id>bt1fDSTLzc1AdSoxXT120380128309808080jWYHN2H</id>
    </if>
  </endorser>
</condition>
)";

  std::string endorser_condition_2 = R"(
<condition id="test">
  <endorser eval-rule="and">
    <id>bt1fDSTLzc1AdSoxXTy6bfAG5NZFHJBThEHjjWYHN2H</id>
    <id>bt1fDSTLzc1AdSoxXTy6bfAG5NZFHJBThEHjjWYHN3H</id>
  </endorser>
</condition>
)";

  pugi::xml_document doc;
  pugi::xml_document doc2;

  if (doc.load_string(endorser_condition.c_str(), pugi::parse_minimal)) {
    pugi::xml_node doc_node = doc.document_element();

    ConditionManager condition_manager;
    DataManager data_manager;

    data_manager.updateValue("$tx.endorser.count", "1");
    data_manager.updateValue("$tx.endorser[0].id", "bt1fDSTLzc1AdSoxXTy6bfAG5NZFHJBThEHjjWYHN2H");

    BOOST_TEST(condition_manager.evalue(doc_node,data_manager));

  } else {
    BOOST_TEST(false);
  }

  if (doc2.load_string(endorser_condition_2.c_str(), pugi::parse_minimal)) {
    pugi::xml_node doc_node = doc2.document_element();

    ConditionManager condition_manager;
    DataManager data_manager;

    data_manager.updateValue("$tx.endorser.count", "1");
    data_manager.updateValue("$tx.endorser[0].id", "bt1fDSTLzc1AdSoxXTy6bfAG5NZFHJBThEHjjWYHN2H");

    BOOST_TEST(!condition_manager.evalue(doc_node,data_manager));

  } else {
    BOOST_TEST(false);
  }

}

BOOST_AUTO_TEST_CASE(simple_user) {

  std::string time_condition = R"(
<condition id="test">
  <user>
    <id>bt1fDSTLzc1AdSoxXTy6bfAG5NZFHJBThEHjjWYHN2H</id>
    <age after="15" before="99" />
  </user>
</condition>
)";

  pugi::xml_document doc;

  if (doc.load_string(time_condition.c_str(), pugi::parse_minimal)) {
    pugi::xml_node doc_node = doc.document_element();

    ConditionManager condition_manager;
    DataManager data_manager;

    data_manager.updateValue("$time", "1559118164"); // 2019-05-29T17:22:44+09:00
    data_manager.updateValue("$user", "bt1fDSTLzc1AdSoxXTy6bfAG5NZFHJBThEHjjWYHN2H");
    data_manager.updateValue("$user.birthday", "1980-08-15");

    BOOST_TEST(condition_manager.evalue(doc_node,data_manager));

  } else {
    BOOST_TEST(false);
  }

}

BOOST_AUTO_TEST_CASE(simple_time) {

  std::string time_condition = R"(
<condition id="test">
  <time>
    <after>2019-01-01T00:00:01</after>
    <before>2019-02-01T23:59:59</before>
  </time>
</condition>
)";

  pugi::xml_document doc;

  if (doc.load_string(time_condition.c_str(), pugi::parse_minimal)) {
    pugi::xml_node doc_node = doc.document_element();

    ConditionManager condition_manager;
    DataManager data_manager;

    data_manager.updateValue("$time", "2019-01-02 23:59:59");

    BOOST_TEST(condition_manager.evalue(doc_node,data_manager));

  } else {
    BOOST_TEST(false);
  }

}

BOOST_AUTO_TEST_CASE(simple_certificate) {

  std::string cert_certificate = R"(
<condition id="test">
  <certificate>
    <pk value="$.pk" type="PEM"></pk>
    <by type="PEM">
      -----BEGIN CERTIFICATE-----
      MIIFEDCCAz+gAwIBAgIRAL2RcbL4kFwgq5ypdBQ8AxwwRgYJKoZIhvcNAQEKMDmg
      DzANBglghkgBZQMEAgEFAKEcMBoGCSqGSIb3DQEBCDANBglghkgBZQMEAgEFAKID
      AgEgowMCAQEwPTEVMBMGA1UEAxMMLy8vLy8vLy8vLzg9MQswCQYDVQQGEwJLUjEX
      MBUGA1UEChMOR3J1dXQgTmV0d29ya3MwHhcNMTgxMjE1MTMzMDUzWhcNMjgxMjEy
      MTMzMDUzWjA9MRUwEwYDVQQDEwwvLy8vLy8vLy8vOD0xCzAJBgNVBAYTAktSMRcw
      FQYDVQQKEw5HcnV1dCBOZXR3b3JrczCCAaIwDQYJKoZIhvcNAQEBBQADggGPADCC
      AYoCggGBALSTWl5lJH6XmBb/SpI5QGvSfljGA5RucagZZoSMGdIPiCMBsfci9BwG
      ZiYL8jY5+og7FZMrB6d7QzgjNm0oaLJU4rV+T5u+EExnWUliXHLqgjiU5gFtcqB7
      vmstBtKuh2dQpD1ZxhOiBuQhaqWG3pfuBfltyx7qR4WaEZzFzsCW5OH67OU5zTQf
      fA3dEscgXFWQ07tGHzkfgq57SqDq8cSoyLmyBsFdLEELou8qVrhH5w9fHsme2n+B
      lEegZmbc24lueRXxGv42DZO9DL21L1GpLNxSFgj9FKz4YbK2Jhx/dtTrOrg+Yled
      HViEYKka13jlLPgln64R1aAyy40jpWhjmpHYmJqwscoQ0DTQEC/vEUGHJJrAsW25
      1LnTYB/yB+VWLuAFueZFYm8AHvyt93sOin90OXpSV1tXkjoz7/2Rem5SMVQPlnyR
      gm68xxoDgNMOauFZ5JmSgx5sPFHHxwlxwKOyw1EAN9XOVtVGl6w9mXPjbzu+pDfC
      SkOM+mtBlwIDAQABo4GYMIGVMCEGA1UdDgQaBBgYJqo0iW9v/30tECWTkkYcGNgp
      5t6Ms/0wDgYDVR0PAQH/BAQDAgEGMCcGA1UdEQQgMB6BEWNvbnRhY3RAZ3J1dXQu
      bmV0gglncnV1dC5uZXQwEgYDVR0TAQH/BAgwBgEB/wIBATAjBgNVHSMEHDAagBgY
      Jqo0iW9v/30tECWTkkYcGNgp5t6Ms/0wRgYJKoZIhvcNAQEKMDmgDzANBglghkgB
      ZQMEAgEFAKEcMBoGCSqGSIb3DQEBCDANBglghkgBZQMEAgEFAKIDAgEgowMCAQED
      ggGBAIeyCjLC5Y689nLGp+LrFFz5T2O2B4Va+rP2txsfcwlbHKEERuB+QA/uPI+s
      9UJboSjLMH62XINa1XKBEqAvzJDX9L2te37nOLREU7I/Qvrym2NqpgMv9Np2NQwp
      G814DZ5eU15Ucopwxm5B8FLn3efjYly+kyv759wYQZfbdHtrh+5BndwfH7r7i3u+
      Fxtgw0pOwgWExYlEF7spnH8KxbvBlXwc3u3zBNsW4gJrABjMVGald/Ix3N+ajDFb
      eU61gFtL1FdvE0tTNJm+MN4P97p9Mkhvkf79jnbFIIP+/XPF57e6xMVjtktHzJTH
      O7l7rUySeRKo061enzCnoLgx/DAPdsy6S8vTDguQmvH9+hk2a6L2+4TyI0oC0hmc
      dAarJFQPV4VGr5Eczs8cqVMBlzLsAMmj0QoFh3S+cTdgWxjkxrHbe2IHIXj2MJUZ
      oJI4+Emvh0kf8C57nJ/5/oG/GP+MYWZmiXJhwrfhz/wgdgxAkPSDxNLfGB7n12Dt
      AA1+pw==
      -----END CERTIFICATE-----
    </by>
  </certificate>
</condition>
)";

  pugi::xml_document doc;

  if (doc.load_string(cert_certificate.c_str(), pugi::parse_minimal)) {
    pugi::xml_node doc_node = doc.document_element();

    ConditionManager condition_manager;
    DataManager data_manager;

    std::string certicate_str = R"(
-----BEGIN CERTIFICATE-----
MIIDMDCCAZigAwIBAgIRAOKO1fHMKM2wJiFW8BoKF/4wDQYJKoZIhvcNAQELBQAw
PTEVMBMGA1UEAxMMLy8vLy8vLy8vLzg9MQswCQYDVQQGEwJLUjEXMBUGA1UEChMO
R3J1dXQgTmV0d29ya3MwHhcNMTkwMTA0MDU1NzI4WhcNMjkwMTAxMDU1NzI4WjA9
MRUwEwYDVQQDEwxUVVZTUjBWU0xURT0xCzAJBgNVBAYTAktSMRcwFQYDVQQKEw5H
cnV1dCBOZXR3b3JrczBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABPjiXGecyzz6
rSSIrYysgi6x+AmeJdCAvp0tA/6gHPlkJSrCRomOhc1CEn4o0snV2INbsoOZFk+0
/+eGgl6daxmjdjB0MCEGA1UdDgQaBBhJ9/9a4Sii228d56Bdci280G5U24Q7PLYw
HAYDVR0RBBUwE4ERY29udGFjdEBncnV1dC5uZXQwDAYDVR0TAQH/BAIwADAjBgNV
HSMEHDAagBgYJqo0iW9v/30tECWTkkYcGNgp5t6Ms/0wDQYJKoZIhvcNAQELBQAD
ggGBAInl5sBETRGxd8M023BzDKngKeip4ojtPBjE5wOu0O3tSo+6Z3rob1u0Br6o
gQamb6K3xynsLH4i3oKVMf/3oVdMtuxNYLio6RxsZyjS7r5u1u5OaA6Dk5DlQHqN
PnGXX5/q6HHdmqlo7KkllmjHkAxCshZqxmXO3XA+FkVN0T/rbjsk4X2xdq8dAG8T
b083TNf6htrT0fSzy96kcnyG26MnO/SVIlH6XncukZuHg7WtFDGuyvvfB6DTYxaW
fc/Ay1DtIdPNlb4BasvSh3KkSlgOGluDIMnj2Z5DcvYV1JF1JedNiqC+aiWth/m9
/id1/WXBAuWPXJxdJg+txN65bfLxeY5L8iNcAONYL1jX3fzZLBaLVuh9dV6fSo5x
MIQ9mHMhTUrCq+lbV5ihwTwYBAk8zJJoD/M6V/d/2imvMSU4vWMK2bWqj035fYWo
wJqkw2WHOoNK3DJHznphy9LiuwmV9ZQY3Z2LO3wJcIS5YqYQrEczbzyGKS0F3hz7
3L1vcg==
-----END CERTIFICATE-----
)";

    data_manager.updateValue("$.pk", certicate_str);

    BOOST_TEST(condition_manager.evalue(doc_node,data_manager));

  } else {
    BOOST_TEST(false);
  }

}



BOOST_AUTO_TEST_SUITE_END()