import requests
from selenium import webdriver
from webdriver_manager.firefox import GeckoDriverManager
from selenium.webdriver.firefox.service import Service
from selenium.webdriver.firefox.options import Options
from selenium.webdriver.common.by import By

getbinary = lambda x, n: format(x, 'b').zfill(n)

def add_parity_bit(key):
    bin_key = getbinary(key, 64)
    K = ''
    for octet in range(8):
        key_octet = bin_key[(8 * octet) : ((8 * octet) + 8)]
        if key_octet.count('1') % 2 == 0:
            key_octet = key_octet[:-1] + '1'

        K = K + key_octet

    print("La clé secrète est: ")
    print("{0:0>4X}".format(int(K, 2)))

def attack_DES(key: str, driver):
    clair = 'A1F8ADF7767C5B6D'
    chiffre = '35D12C82F252DC5B'

    driver.get(f"https://emvlab.org/descalc/?key={key}&iv=0000000000000000&input={clair}&mode=ecb&action=Encrypt&output=")
    elem = driver.find_element(by=By.XPATH, value='/html/body/center/table/tbody/tr[2]/td[2]/div[2]/form/table/tbody/tr/td[2]/input[1]')
    elem.click()
    res = driver.find_element(by=By.XPATH, value='//*[@id="output"]')

    if res.text == chiffre:
        add_parity_bit(int(key.replace('\n', ''), 16))
        return True
    
    return False
            

if __name__ == '__main__':
    options = Options()
    options.add_argument('--headless')
    s = Service(GeckoDriverManager().install())
    driver = webdriver.Firefox(service=s, options=options)

    with open("potential_K.txt", 'r') as file:
        lignes = file.readlines()
        for ligne in lignes:
            if attack_DES(ligne, driver):
                break
    
    driver.close()
