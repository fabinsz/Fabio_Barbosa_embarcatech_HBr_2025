#  Projeto de Teste Unitário em C com Unity

Este projeto mostra como realizar **testes unitários** em C usando a biblioteca [Unity](https://github.com/ThrowTheSwitch/Unity).  
O foco é testar uma função que converte valores do ADC (conversor analógico-digital) para temperatura em Celsius.

---

A função adc_to_celsius é testada com um valor de ADC que representa 0.706 V, que, segundo o datasheet do RP2040, corresponde a uma temperatura de 27°C.

O teste valida se o resultado está dentro de uma margem de erro de ±0.1°C:
```
TEST_ASSERT_FLOAT_WITHIN(0.1, 27.0, temp);
```

## Executar
1. Abra um terminal e navegue até a pasta do projeto.
2. Execute este comando:
```
gcc -o testes_temp test_temp_conversion.c temp_utils.c unity/unity.c -Iunity

./testes_temp
```