#!/usr/bin/python3
import re
import csv

import requests
from bs4 import BeautifulSoup


def spracuj_vyjazd(url, csv):
    ret = {'Podtyp':'','Nahlasenie':'','Lokalizacia':'','Uhasenie':'','Sprava':''}
    r = requests.get('http://www.firebrno.cz/modules/incidents/'+str(url))
    soup = BeautifulSoup(r.text, "html.parser")
    incident = soup.find('div','inc-detail')
    ret['Nahlasenie'] = re.findall('[0-9]+:[0-9]+',(incident.p).get_text())[0]
    for tag in incident('p'):

        i = tag.get_text().find(":")
        part = [(tag.get_text())[:i],(tag.get_text())[i + 1:]]
        if 'Podtyp' in part[0]:
            ret['Podtyp'] = part[1]
        elif 'Popis' in part[0]:
            ret['Sprava'] = part[1]
            casi = re.findall('[0-9]+:[0-9]+', part[1])
            if (len(casi)) == 1:
                ret['Lokalizacia'] = casi[0]
                ret['Uhasenie'] = casi[0]
            elif (len(casi)) == 2:
                ret['Lokalizacia'] = casi[0]
                ret['Uhasenie'] = casi[1]
    csv.writerow(ret)



def main():
    with open('vyjazdy.csv', 'w') as csvfile:
        fieldnames = ['Podtyp', 'Nahlasenie','Lokalizacia','Uhasenie','Sprava']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for i in range(1,32):
            r = requests.get('http://www.firebrno.cz/modules/incidents/index.php?filter%5Bdate%5D=&filter%5Btype_id%5D=1&filter%5Bdistrict_id%5D=3702&search=Vyhledat&page='+str(i))
            soup = BeautifulSoup(r.text, "html.parser")
            for vyjazd in soup.find_all(title='Detail')[::2]:
                spracuj_vyjazd(vyjazd.get('href'), writer)

if __name__ == "__main__":
    main()